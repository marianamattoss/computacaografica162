#include <vector>
#include "point.hpp"

Point::Point(string name, Coordinate coord) :
		GeometricObject(name, point, {coord} ) {
}

void Point::applyClipping() {
	_clippedObjects = Clipper::clipPoint(_windowCoords[0]);
}
