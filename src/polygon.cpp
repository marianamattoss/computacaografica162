#include "polygon.hpp"

Polygon::Polygon(string name, vector<Coordinate> coords) :
		GeometricObject(name, polygon, coords) {}
