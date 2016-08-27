/*
 * cubic_bezier.h
 *
 *  Created on: 27 de abr de 2016
 *      Author: plab
 */

#ifndef SRC_CUBIC_BEZIER_H_
#define SRC_CUBIC_BEZIER_H_

#include <math.h>

#include "geometric_object.hpp"
#include "clipper.hpp"

using namespace std;

class CubicBezier : public GeometricObject {
public:
	CubicBezier(string name, vector<Coordinate> controlPoints, double step);
	virtual ~CubicBezier();

	virtual void applyClipping();

private:
	void _generatePoints(vector<Coordinate> controlPoints, double step);

	Matrix<1,4, double> _createPowersMatrix(double t);
	vector< Matrix<4,1, Coordinate> > _createGeometryMatrices(vector<Coordinate> controlPoints);
	Matrix<4,4, double> _createBezierMatrix();
};

#endif /* SRC_CUBIC_BEZIER_H_ */
