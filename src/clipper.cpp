#include "clipper.hpp"

Clipper::Clipper() {}
Clipper::~Clipper() {}

ClippingFunction Clipper::clippingFunction = &Clipper::cohenSutherland;

bool Clipper::isOutOfRange(Coordinate coord) {
	return fabs(coord._x) > 1 || fabs(coord._y) > 1;
}

vector<CLIPPED_OBJECT> Clipper::clipPoint(Coordinate coord) {
	if (isOutOfRange(coord))
		return vector<CLIPPED_OBJECT>();

	return { {coord} };
}

unsigned int Clipper::regionCodeOf(Coordinate coord) {
	unsigned int code = 0;

	//Using bitwise operators to represent region code.
	code = (coord._y > SCN_MAX) ? (code | UP) : code;
	code = (coord._y < SCN_MIN) ? (code | DOWN) : code;
	code = (coord._x > SCN_MAX) ? (code | RIGHT) : code;
	code = (coord._x < SCN_MIN) ? (code | LEFT) : code;

	return code;
}

CLIPPED_OBJECT Clipper::cohenSutherland(vector<Coordinate> coords) {
	unsigned int rcBegin = regionCodeOf(coords[0]);
	unsigned int rcEnd = regionCodeOf(coords[1]);

	bool insideWindow = !rcBegin && !rcEnd;
	if (insideWindow) 
		return coords;

	bool partiallyInside = (rcBegin != rcEnd) && ( !(rcBegin & rcEnd) );
	if (partiallyInside)
		return applyCohenSutherland(coords, {rcBegin, rcEnd});
	
	return CLIPPED_OBJECT();
}

CLIPPED_OBJECT Clipper::applyCohenSutherland(vector<Coordinate> coords, vector<unsigned int> regionCodes) {
	double m = (coords[1]._y - coords[0]._y) / (coords[1]._x - coords[0]._x);

	vector<Coordinate> clippedCoords;
	double x,y;

	for (int i = 0; i < coords.size(); ++i) {

		if (regionCodes[i] & LEFT) {
			y = m * (SCN_MIN - coords[i]._x) + coords[i]._y;
			clippedCoords.push_back( Coordinate(SCN_MIN, y) );

			continue;
		}
		if (regionCodes[i] & RIGHT) {
			y = m * (SCN_MAX - coords[i]._x) + coords[i]._y;
			clippedCoords.push_back( Coordinate(SCN_MAX, y) );

			continue;
		}
		if (regionCodes[i] & UP) {
			x = coords[i]._x + 1 / m * (SCN_MAX - coords[i]._y);
			clippedCoords.push_back( Coordinate(x, SCN_MAX) );

			continue;
		}
		if (regionCodes[i] & DOWN) {
			x = coords[i]._x + 1 / m * (SCN_MIN - coords[i]._y);
			clippedCoords.push_back( Coordinate(x, SCN_MIN) );

			continue;
		}

		clippedCoords.push_back(coords[i]);
	}

	return clippedCoords;
}

CLIPPED_OBJECT Clipper::liangBarsky(vector<Coordinate> coords) {
	vector<Coordinate> clippedCoords;

	double dx = coords[1]._x - coords[0]._x;
	double dy = coords[1]._y - coords[0]._y;

	vector<double> p = {-dx, dx, -dy, dy};
	vector<double> q = {coords[0]._x - SCN_MIN,
		SCN_MAX - coords[0]._x,
		coords[0]._y - SCN_MIN,
		SCN_MAX - coords[0]._y};

	bool outsideWindow = (!p[0] && q[0] < 0) || (!p[2] && q[2] < 0) ||
						(!p[1] && q[1] < 0) || (!p[3] && q[3] < 0);

	if (outsideWindow)
		return CLIPPED_OBJECT();

	double x, y;
	vector<double> coefs = calculateCoeficients(p, q);

	if (coefs[0] > coefs[1])
		return CLIPPED_OBJECT();
	
	if (coefs[0] > 0) {
		x = coords[0]._x + coefs[0] * dx;
		y = coords[0]._y + coefs[0] * dy;

		clippedCoords.push_back( Coordinate(x,y) );
	} else {
		clippedCoords.push_back( coords[0] );
	}

	if (coefs[1] < 1) {
		x = coords[0]._x + coefs[1] * dx;
		y = coords[0]._y + coefs[1] * dy;

		clippedCoords.push_back( Coordinate(x,y) );
	} else {
		clippedCoords.push_back( coords[1] );
	}

	return clippedCoords;
}

vector<CLIPPED_OBJECT> Clipper::clipCurve(vector<Coordinate> coords) {
	vector<CLIPPED_OBJECT> clippedObjects;
	CLIPPED_OBJECT currentObject, clippedLine;

	int firstIndex = 0;
	bool coordIn, clipIn;

	for (int i = 0; i < coords.size(); ++i) {
		coordIn = !isOutOfRange(coords[i]);
		clipIn = ( (i - firstIndex) > 0 ) && currentObject.empty() && coordIn;

		if (clipIn) {
			//Clip out in
			clippedLine = cohenSutherland({coords[i-1], coords[i]});

			currentObject.push_back(clippedLine[0]);
			currentObject.push_back(clippedLine[1]);
		} else if (coordIn) {
			// coord in, don't need to clip
			currentObject.push_back(coords[i]);
		} else if ( !currentObject.empty() ) {
			//Clip in out
			clippedLine = cohenSutherland({currentObject.back(), coords[i]});
			currentObject.push_back(clippedLine[1]);

			clippedObjects.push_back(currentObject);
			currentObject = CLIPPED_OBJECT();

			//First index of new clipped-object
			firstIndex = i + 1;
		}
	}

	if (!currentObject.empty())
		clippedObjects.push_back(currentObject);

	return clippedObjects;
}

vector<CLIPPED_OBJECT> Clipper::weilerAtherton(vector<Coordinate> objectCoords) {
	list<ClippingPoint> object, clip, gettingIn;
	list<ClippingPoint>::iterator currentObjVertex, nextObjectVertex, currentWindowVertex, nextWindowVertex;
	vector<Coordinate> currentObjLine, currentWindowLine;
	Coordinate intersection;

	vector<Coordinate> windowCoords = {Coordinate(-1,1), Coordinate(1,1), Coordinate(1,-1), Coordinate(-1,-1)};

	for(Coordinate vertex : objectCoords) {
		object.push_back(ClippingPoint(vertex));
	}

	for(Coordinate vertex : windowCoords) {
		clip.push_back(ClippingPoint(vertex));
	}

	for(currentObjVertex = object.begin(); currentObjVertex != object.end(); currentObjVertex++) {

		if(!currentObjVertex->isArtificial()) {
			nextObjectVertex = next(currentObjVertex, 1);
			while(nextObjectVertex->isArtificial()) {
				nextObjectVertex = next(nextObjectVertex, 1);
			}

			currentObjLine = {Coordinate(currentObjVertex->coord()._x, currentObjVertex->coord()._y), 
							  Coordinate(nextObjectVertex->coord()._x, nextObjectVertex->coord()._y)};

			// if(isOutOfRange(currentObjVertex->coord()) != isOutOfRange(nextObjectVertex->coord())) {
				for(currentWindowVertex = clip.begin(); currentWindowVertex != clip.end(); currentWindowVertex++) {
					if(!currentWindowVertex->isArtificial()) {
						nextWindowVertex = next(currentWindowVertex, 1);
						while(nextWindowVertex->isArtificial()) {
							nextWindowVertex = next(nextWindowVertex, 1);
						}

						currentWindowLine = {Coordinate(currentWindowVertex->coord()._x, currentWindowVertex->coord()._y), 
											 Coordinate(nextWindowVertex->coord()._x, nextWindowVertex->coord()._y)};
						
						if(hasIntersection(currentObjLine, currentWindowLine, intersection)) {
							ClippingPoint point = ClippingPoint(intersection, true);

							// cout << "Adicionando (" << point.coord()._x << ", " << point.coord()._y << ")\n";

							if(nextObjectVertex == object.begin()) {
								object.push_back(point);
							} else {
								if(currentObjVertex->coord()._x < -1.0)
									object.insert(next(currentObjVertex, 1), point);
								else
									object.insert(nextObjectVertex, point);
							}

							if(nextWindowVertex == clip.begin()) {
								clip.insert(next(currentWindowVertex, 1), point);
							} else {
								clip.insert(nextWindowVertex, point);
							}

							if(isOutOfRange(currentObjLine[0]) && !isOutOfRange(currentObjLine[1])) {
								gettingIn.push_back(point);
							}
						}
					}								
				}
			// }
		}
	}

	return weilerAthertonPolygonGenerator(objectCoords, gettingIn, object, clip);

	// Printing
	// int i = 1;
	// for(currentObjVertex = object.begin(); currentObjVertex != object.end(); currentObjVertex++) {
	// 	cout << "V#" << i << " (" << currentObjVertex->coord()._x << ", " << currentObjVertex->coord()._y << ")" << endl;
	// 	i++;
	// }

	// cout << endl;

	// i = 1;
	// for(currentWindowVertex = clip.begin(); currentWindowVertex != clip.end(); currentWindowVertex++) {
	// 	cout << "W#" << i << " (" << currentWindowVertex->coord()._x << ", " << currentWindowVertex->coord()._y << ")" << endl;
	// 	i++;
	// }

	// cout << endl;

	// for(currentWindowVertex = gettingIn.begin(); currentWindowVertex != gettingIn.end(); currentWindowVertex++) {
	// 	cout << "GI#" << " (" << currentWindowVertex->coord()._x << ", " << currentWindowVertex->coord()._y << ")" << endl;
	// }

	// cout << endl;
}

/**
 * Performs a navigation though lists, in order to mount the final object.
 */
vector<CLIPPED_OBJECT> Clipper::weilerAthertonPolygonGenerator(vector<Coordinate> objectCoords, 
	list<ClippingPoint> gettingIn, list<ClippingPoint> object, list<ClippingPoint> clip) {
	
	if(gettingIn.empty()) {return { objectCoords };}

	vector<CLIPPED_OBJECT> final;
	vector<Coordinate> temp;
	list<ClippingPoint>* container;
	list<ClippingPoint>::iterator currentObjVertex, iter;

	bool firstIterationOnList;
	bool objectCompleted = false;

	for(iter = gettingIn.begin(); iter != gettingIn.end(); iter++) {
		ClippingPoint referenceObject = *iter;	
		container = &object;

		while(!objectCompleted) {
			firstIterationOnList = true;

			for(currentObjVertex = getIterator(referenceObject, container); currentObjVertex != container->end(); currentObjVertex++) {
				if(firstIterationOnList) {
					temp.push_back(Coordinate(currentObjVertex->coord()._x, currentObjVertex->coord()._y));
					firstIterationOnList = false;
				} else {
					if(currentObjVertex->isArtificial()) {
						if(container == &object)
							container = &clip;
						else
							container = &object;

						if(sameCoordinates(temp.front(), currentObjVertex->coord()))
							objectCompleted = true;

						referenceObject = *currentObjVertex;
						break;
					} else {
						temp.push_back(Coordinate(currentObjVertex->coord()._x, currentObjVertex->coord()._y));
					}
				}

				if(next(currentObjVertex, 1) == container->end()) {
					temp.push_back(Coordinate(container->begin()->coord()._x, container->begin()->coord()._y));
					currentObjVertex = container->begin();
				}
			}
		}

		final.push_back(temp);
	}

	// for(currentObjVertex = final.begin(); currentObjVertex != final.end(); currentObjVertex++) {
	// 	result.push_back(currentObjVertex->coord());
	// 	cout << "F#" << " (" << currentObjVertex->coord()._x << ", " << currentObjVertex->coord()._y << ")" << endl;
	// }

	return final;
}

vector<double> Clipper::calculateCoeficients(vector<double> p, vector<double> q) {
	vector<double> coefs;

	//Calculo dos valores para coordenada 1
	double r1 = (p[0] < 0) ? (q[0] / p[0]) : (q[1] / p[1]);
	double r2 = (p[2] < 0) ? (q[2] / p[2]) : (q[3] / p[3]);

	r1 = (!isfinite(r1)) ? 0.0 : r1;
	r2 = (!isfinite(r2)) ? 0.0 : r2;

	coefs.push_back( max(0.0, max(r1,r2)) );

	//Calculo dos valores para coordenada 2
	r1 = (p[1] < 0) ? (q[0] / p[0]) : (q[1] / p[1]);
	r2 = (p[3] < 0) ? (q[2] / p[2]) : (q[3] / p[3]);

	r1 = (!isfinite(r1)) ? 1.0 : r1;
	r2 = (!isfinite(r2)) ? 1.0 : r2;

	coefs.push_back( min(1.0, min(r1,r2)) );	

	return coefs;	
}

bool Clipper::hasIntersection(vector<Coordinate> firstLine, vector<Coordinate> secondLine, 
							  Coordinate &intersection) {
	Coordinate a = Coordinate(firstLine[1]._x - firstLine[0]._x, firstLine[1]._y - firstLine[0]._y);
	Coordinate b = Coordinate(secondLine[1]._x - secondLine[0]._x, secondLine[1]._y - secondLine[0]._y);

	double f = a.perpDotProduct(b);

	if(!f)
		return false;
	
	Coordinate c = Coordinate(secondLine[1]._x - firstLine[1]._x, secondLine[1]._y - firstLine[1]._y);
	
	double aa = a.perpDotProduct(c);
	double bb = b.perpDotProduct(c);

	if(f < 0) {
		if(aa > 0) return false;
		if(bb > 0) return false;
		if(aa < f) return false;
		if(bb < f) return false;
	} else {
		if(aa < 0) return false;
		if(bb < 0) return false;
		if(aa > f) return false;
		if(bb > f) return false;
	}

	double coef = 1.0 - (aa / f);
	intersection = Coordinate(((secondLine[1]._x - secondLine[0]._x) * coef) + secondLine[0]._x, 
							  ((secondLine[1]._y - secondLine[0]._y) * coef) + secondLine[0]._y);

	return true;
}

list<ClippingPoint>::iterator Clipper::getIterator(ClippingPoint object, list<ClippingPoint>* container) {
	list<ClippingPoint>::iterator iter;
	for(iter = container->begin(); iter != container->end(); iter++) {
		if((ClippingPoint*) iter->equals(object))
			return iter;
	}

	return iter;
}

bool Clipper::sameCoordinates(Coordinate a, Coordinate b) {
	return a._x == b._x && a._y == b._y;
}
