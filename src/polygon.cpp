#include "polygon.hpp"

Polygon::Polygon(string name, vector<Coordinate> coords) :
		GeometricElement(name, polygon, coords) {}

void Polygon::applyClipping() {		
	_clippedObjects = Clipper::weilerAtherton(coords());		
}