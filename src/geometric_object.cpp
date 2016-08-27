#include "geometric_object.hpp"

GeometricObject::GeometricObject(string name, GeometricObjectType type,
		vector<Coordinate> coords) :
		_name(name), _type(type), _worldCoords(coords) {
}

GeometricObject::~GeometricObject() {
}

string GeometricObject::name() const {
	return _name;
}

GeometricObjectType GeometricObject::type() const {
	return _type;
}

vector<Coordinate> GeometricObject::coords() const {
	return _windowCoords;
}

vector<Coordinate> GeometricObject::worldCoords() const {
	return _worldCoords;
}

Coordinate GeometricObject::centroid() const {
	double new_x = 0, new_y = 0;

	for(auto coord : _worldCoords) {
		new_x += coord._x;
		new_y += coord._y;
	}

	new_x = new_x / _worldCoords.size();
	new_y = new_y / _worldCoords.size();

	return Coordinate(new_x, new_y);
}

void GeometricObject::normalizeIn(Window window) {
	Coordinate windowCenter = window.center();
	SQUARE_MATRIX transformation = window.normalizedTransformation();
	ROW_VECTOR result;

	double xOffset = window.xOffset();
	double yOffset = window.yOffset();
	double x,y;

	int numCoords = _worldCoords.size();

	_windowCoords.clear();

	for (int i = 0; i < numCoords; ++i) {
		result = _worldCoords[i].toHomogenousMatrix() * transformation;

		x = ( result.valueOn(0,0) - windowCenter._x ) / (xOffset);
		y = ( result.valueOn(0,1) - windowCenter._y ) / (yOffset);

		_windowCoords.push_back( Coordinate(x,y) );
	}
}

void GeometricObject::translate(VECTOR deslocation) {
	SQUARE_MATRIX translationMatrix = _buildTranslationMatrix(deslocation);
	applyTransformation(translationMatrix);
}

void GeometricObject::scaleTo(VECTOR factors) {
	SQUARE_MATRIX scaleMatrix = _buildScaleMatrix(factors._x, factors._y);
	positionBasedTransformation(scaleMatrix, centroid());
}

void GeometricObject::rotate(double radians) {
	SQUARE_MATRIX rotationMatrix = _buildRotationMatrix(radians);
	positionBasedTransformation(rotationMatrix, centroid());
}

void GeometricObject::rotate(double radians, Coordinate anchor) {
	SQUARE_MATRIX rotationMatrix = _buildRotationMatrix(radians);
	positionBasedTransformation(rotationMatrix, anchor);
}

void GeometricObject::applyTransformation(SQUARE_MATRIX transfMatrix) {
	ROW_VECTOR result;

	int numCoords = _worldCoords.size();

	for (int i = 0; i < numCoords; ++i) {
		result = _worldCoords[i].toHomogenousMatrix() * transfMatrix;

		_worldCoords[i]._x = result.valueOn(0,0);
		_worldCoords[i]._y = result.valueOn(0,1);
	}

}

void GeometricObject::positionBasedTransformation(SQUARE_MATRIX targetTransformation, Coordinate coord) {
	Coordinate originDeslocation(-coord._x, -coord._y);

	SQUARE_MATRIX operationMatrix = _buildTranslationMatrix(originDeslocation) *
			targetTransformation *
			_buildTranslationMatrix(originDeslocation.negate());

	applyTransformation(operationMatrix);
}
