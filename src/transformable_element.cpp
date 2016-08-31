#include "transformable_element.hpp"

TransformableElement::TransformableElement() {

}

TransformableElement::~TransformableElement() {
	
}

// Protected functions Homogeneous Coordinates

SQUARE_MATRIX TransformableElement::_buildTranslationMatrix(VECTOR deslocation) {
	SQUARE_MATRIX matrix = SQUARE_MATRIX::buildIdentity();

	matrix.setValueOn(2, 0, deslocation._x);
	matrix.setValueOn(2, 1, deslocation._y);

	return matrix;
}

SQUARE_MATRIX TransformableElement::_buildScaleMatrix(double x_factor, double y_factor) {
	SQUARE_MATRIX matrix = SQUARE_MATRIX::buildIdentity();

	matrix.setValueOn(0, 0, x_factor);
	matrix.setValueOn(1, 1, y_factor);

	return matrix;
}

SQUARE_MATRIX TransformableElement::_buildRotationMatrix(double radians) {
	SQUARE_MATRIX matrix = SQUARE_MATRIX::buildIdentity();

	matrix.setValueOn(0, 0, cos(radians));
	matrix.setValueOn(0, 1, sin(radians));
	matrix.setValueOn(1, 0, -sin(radians));
	matrix.setValueOn(1, 1, cos(radians));

	return matrix;
}
