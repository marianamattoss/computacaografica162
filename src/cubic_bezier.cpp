/*
 * cubic_bezier.cpp
 *
 *  Created on: 27 de abr de 2016
 *      Author: plab
 */

#include "cubic_bezier.h"

CubicBezier::CubicBezier(string name, vector<Coordinate> controlPoints, double step) :
	GeometricObject(name, cubic_bezier, vector<Coordinate>()) {

	_generatePoints(controlPoints, step);
}

CubicBezier::~CubicBezier() {
	// TODO Auto-generated destructor stub
}

void CubicBezier::applyClipping() {
	_clippedObjects = Clipper::clipCurve(_windowCoords);
	//_clippedObjects = { _windowCoords };
}

void CubicBezier::_generatePoints(vector<Coordinate> controlPoints, double step) {
	vector< Matrix<4,1, Coordinate> > geometryMatrices = _createGeometryMatrices(controlPoints);
	Matrix<4,4, double> bezierMatrix = _createBezierMatrix();

	Matrix<1,1,Coordinate> coordMatrix;
	Coordinate coord;

	for (int i = 0; i < geometryMatrices.size(); ++i) {
		for (double t = 0; t <= 1; t += step) {
			//coordMatrix = T * Mb * Gb
			coordMatrix = _createPowersMatrix(t) * bezierMatrix * geometryMatrices[i];
			coord = coordMatrix.valueOn(0,0);

			_worldCoords.push_back(coord);
		}
	}
}

Matrix<1,4, double> CubicBezier::_createPowersMatrix(double t) {
	Matrix<1,4, double> powersMatrix;

	powersMatrix.setValueOn(0,0, pow(t, 3));
	powersMatrix.setValueOn(0,1, pow(t, 2));
	powersMatrix.setValueOn(0,2, t);
	powersMatrix.setValueOn(0,3, 1);

	return powersMatrix;
}

vector< Matrix<4,1, Coordinate> > CubicBezier::_createGeometryMatrices(vector<Coordinate> controlPoints) {
	vector< Matrix<4,1, Coordinate> > geometryMatrices;
	Matrix<4,1, Coordinate> geometryMatrix;

	int curvesCount = int( (controlPoints.size() - 4) / 3 ) + 1;

	for (int i = 0; i < curvesCount; ++i) {
		geometryMatrix = Matrix<4,1,Coordinate>();
		geometryMatrix.setValueOn(0,0, controlPoints[0 + 3*i]);
		geometryMatrix.setValueOn(0,1, controlPoints[1 + 3*i]);
		geometryMatrix.setValueOn(0,2, controlPoints[2 + 3*i]);
		geometryMatrix.setValueOn(0,3, controlPoints[3 + 3*i]);

		geometryMatrices.push_back(Matrix<4,1,Coordinate>(geometryMatrix));
	}

	return geometryMatrices;
}

Matrix<4,4, double> CubicBezier::_createBezierMatrix() {
	Matrix<4,4,double> bezierMatrix;

	bezierMatrix.setValueOn(0,0,-1);
	bezierMatrix.setValueOn(0,1,3);
	bezierMatrix.setValueOn(0,2,-3);
	bezierMatrix.setValueOn(0,3,1);

	bezierMatrix.setValueOn(1,0,3);
	bezierMatrix.setValueOn(1,1,-6);
	bezierMatrix.setValueOn(1,2,3);
	bezierMatrix.setValueOn(1,3,0);

	bezierMatrix.setValueOn(2,0,-3);
	bezierMatrix.setValueOn(2,1,3);
	bezierMatrix.setValueOn(2,2,0);
	bezierMatrix.setValueOn(2,3,0);

	bezierMatrix.setValueOn(3,0,1);
	bezierMatrix.setValueOn(3,1,0);
	bezierMatrix.setValueOn(3,2,0);
	bezierMatrix.setValueOn(3,3,0);

	return bezierMatrix;
}

