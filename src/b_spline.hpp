/*
 * b_spline.h
 *
 *  Created on: 5 de mai de 2016
 *      Author: plab
 */

#ifndef SRC_B_SPLINE_HPP_
#define SRC_B_SPLINE_HPP_

#include <cmath>

#include "geometric_object.hpp"
#include "clipper.hpp"

class BSpline : public GeometricObject {
public:
	BSpline(string name, vector<Coordinate> controlPoints, double step);
	virtual ~BSpline();

	virtual void applyClipping();

private:
	void _generatePoints(vector<Coordinate> controlPoints, double step);

	Matrix<4,1,Coordinate> _buildGeometryMatrix(vector<Coordinate> controlPoints, int startIndex);
	Matrix<4,4,double> _buildDiffMatrix(double step);
	Matrix<4,4, double> _buildSplineMatrix();
};

#endif /* SRC_B_SPLINE_HPP_ */
