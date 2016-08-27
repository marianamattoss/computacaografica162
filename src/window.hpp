#ifndef SRC_WINDOW_H_
#define SRC_WINDOW_H_

#include <algorithm>
#include <cmath>
#include <iostream>

#include "transformable_object.hpp"

#define PI 3.14159265358979323846
#define DEG2RAD(DEG) (DEG*PI/180.0)
#define RAD2DEG(RAD) (RAD / PI*180.0)

class Window : public TransformableObject {
public:
	Window(double width, double height);

	void move(Coordinate step);
	void zoom(int step);
	void rotate(double angle);

	Coordinate center();

	double xOffset();
	double yOffset();

	SQUARE_MATRIX normalizedTransformation();

private:
	//Coordinate _lowerLeftCorner, _upperRightCorner;
	Coordinate _center;
	VECTOR _vupVector;

	double _width, _height;
};

#endif /* SRC_WINDOW_H_ */
