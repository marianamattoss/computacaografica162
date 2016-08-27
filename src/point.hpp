#ifndef SRC_GeometricObject_POINT_H_
#define SRC_GeometricObject_POINT_H_

#include <string>
#include <math.h>

#include "geometric_object.hpp"
#include "clipper.hpp"

using namespace std;

class Point : public GeometricObject {

public:
	Point(string name, Coordinate coord);

	virtual void applyClipping();
};

#endif /* SRC_GeometricObject_POINT_H_ */
