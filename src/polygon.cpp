#include "polygon.hpp"

Polygon::Polygon(string name, vector<Coordinate> coords, bool filled) :
		GeometricObject(name, polygon, coords, filled) {}

void Polygon::applyClipping() {
	_clippedObjects = Clipper::weilerAtherton(coords());
}
