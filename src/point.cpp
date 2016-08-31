#include <vector>
#include "point.hpp"

Point::Point(string name, Coordinate coord) :
		GeometricElement(name, point, {coord} ) {
}
