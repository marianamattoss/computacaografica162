#ifndef SRC_TRANSFORMABLE_ELEMENT_HPP_
#define SRC_TRANSFORMABLE_ELEMENT_HPP_

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>

#include "matrix.hpp"

#define VECTOR Coordinate

#define SQUARE_MATRIX Matrix<3,3, double>
#define ROW_VECTOR Matrix<1,3, double>

struct Coordinate {

	Coordinate(double x, double y): _x(x), _y(y) {}
	Coordinate(): _x(0), _y(0) {}

public:
	double _x,_y;

	Coordinate negate(){ return Coordinate(-_x, -_y); }
	ROW_VECTOR toHomogenousMatrix(){
		ROW_VECTOR homogeneousMatrix;
		homogeneousMatrix.setValueOn(0, 0, _x);
		homogeneousMatrix.setValueOn(0, 1, _y);
		homogeneousMatrix.setValueOn(0, 2, 1);

		return homogeneousMatrix;
	}

	double dotProduct( Coordinate other ) {
		return (_x * other._x) + (_y * other._y);
	}

	double perpDotProduct( Coordinate other ) {
		return (_y * other._x) - (_x * other._y);
	}

	double angleWith( Coordinate target ) {
		return acos( dotProduct(target) / (length() * target.length()) );
	}

	double length() {
		return sqrt( pow(_x, 2) + pow(_y, 2) );
	}

	Coordinate operator+( Coordinate other ) {
			return Coordinate( _x + other._x, _y + other._y );
	}

	void operator+=( Coordinate other ) {
		_x += other._x;
		_y += other._y;
	}

	friend Coordinate operator*( Coordinate coord, double scalar ) {
		return Coordinate( coord._x * scalar, coord._y * scalar );
	}

	friend Coordinate operator*( double scalar, Coordinate coord ) {
		return Coordinate( coord._x * scalar, coord._y * scalar );
	}
};

class TransformableElement {
public:
	TransformableElement();
	virtual ~TransformableElement();

protected:
	SQUARE_MATRIX _buildTranslationMatrix(VECTOR deslocation);
	SQUARE_MATRIX _buildScaleMatrix(double x_factor, double y_factor);
	SQUARE_MATRIX _buildRotationMatrix(double radians);
};

#endif /* SRC_TRANSFORMABLE_ELEMENT_HPP_ */
