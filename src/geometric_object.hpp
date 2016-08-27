#ifndef SRC_GEOMETRICOBJECT_H_
#define SRC_GEOMETRICOBJECT_H_

#include <string>

#include "transformable_object.hpp"
#include "window.hpp"

using namespace std;

enum GeometricObjectType {
	point,
	line,
	polygon
};

// ---------------------------

class GeometricObject : public TransformableObject {
public:
	GeometricObject(string name,  GeometricObjectType type, vector<Coordinate> coords);
	GeometricObject(string name,  GeometricObjectType type, vector<Coordinate> coords, bool filled);
	virtual ~GeometricObject();

	string name() const;
	GeometricObjectType type() const;
	vector<Coordinate> coords() const;
	vector<Coordinate> worldCoords() const;
	Coordinate centroid() const;
	bool filled() const;

	void normalizeIn(Window window);

	void translate(VECTOR deslocation);
	void scaleTo(VECTOR factors);
	void rotate(double radians);
	void rotate(double radians, Coordinate anchor);

private:
	void applyTransformation(SQUARE_MATRIX transfMatrix);
	void positionBasedTransformation(SQUARE_MATRIX targetTransformation, Coordinate coord);

protected:
	string _name;
	GeometricObjectType _type;
	vector<Coordinate> _worldCoords;
	vector<Coordinate> _windowCoords;

	bool _filled = false;
};

#endif /* SRC_GEOMETRICOBJECT_H_ */
