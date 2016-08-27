#include <vector>
#include "point.hpp"

Point::Point(string name, Coordinate coord) :
		GeometricObject(name, point, {coord} ) {
}
