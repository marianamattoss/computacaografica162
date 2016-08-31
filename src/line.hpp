#ifndef SRC_LINE_H_
#define SRC_LINE_H_

#include <string>

#include "geometric_element.hpp"

using namespace std;

class Line : public GeometricElement {
public:
	Line(string name, Coordinate first, Coordinate second);
};

#endif /* SRC_LINE_H_ */
