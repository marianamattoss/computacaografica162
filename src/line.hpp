#ifndef SRC_LINE_H_
#define SRC_LINE_H_

#include <string>

#include "geometric_object.hpp"
#include "clipper.hpp"

using namespace std;

class Line : public GeometricObject {
public:
	Line(string name, Coordinate first, Coordinate second);

	virtual void applyClipping();
};

#endif /* SRC_LINE_H_ */
