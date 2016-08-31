#ifndef SRC_POINT_H_
#define SRC_POINT_H_

#include <string>
#include <math.h>

#include "geometric_element.hpp"

using namespace std;

class Point : public GeometricElement {

public:
	Point(string name, Coordinate coord);
};

#endif /* SRC_POINT_H_ */
