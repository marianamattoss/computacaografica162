/*
 * transformable_object.cpp
 *
 *  Created on: 12 de abr de 2016
 *      Author: plab
 */

#include "transformable_object.hpp"

TransformableObject::TransformableObject() {
	// TODO Auto-generated constructor stub

}

TransformableObject::~TransformableObject() {
	// TODO Auto-generated destructor stub
}

// Protected functions Homogeneous Coordinates

SQUARE_MATRIX TransformableObject::_buildTranslationMatrix(VECTOR deslocation) {
	SQUARE_MATRIX matrix = SQUARE_MATRIX::buildIdentity();

	matrix.setValueOn(2, 0, deslocation._x);
	matrix.setValueOn(2, 1, deslocation._y);

	return matrix;
}

SQUARE_MATRIX TransformableObject::_buildScaleMatrix(double x_factor, double y_factor) {
	SQUARE_MATRIX matrix = SQUARE_MATRIX::buildIdentity();

	matrix.setValueOn(0, 0, x_factor);
	matrix.setValueOn(1, 1, y_factor);

	return matrix;
}

SQUARE_MATRIX TransformableObject::_buildRotationMatrix(double radians) {
	SQUARE_MATRIX matrix = SQUARE_MATRIX::buildIdentity();

	matrix.setValueOn(0, 0, cos(radians));
	matrix.setValueOn(0, 1, sin(radians));
	matrix.setValueOn(1, 0, -sin(radians));
	matrix.setValueOn(1, 1, cos(radians));

	return matrix;
}

