#ifndef SRC_GEOMETRICELEMENT_H_
#define SRC_GEOMETRICELEMENT_H_

#include <string>

#include "transformable_element.hpp"
#include "window.hpp"

#define CLIPPED_OBJECT vector<Coordinate>

using namespace std;

enum GeometricElementType {
	point,
	line,
	polygon
};

// ---------------------------

class GeometricElement : public TransformableElement {
public:
	GeometricElement(string name,  GeometricElementType type, vector<Coordinate> coords);
	virtual ~GeometricElement();

	string name() const;
	GeometricElementType type() const;
	vector<Coordinate> coords() const;
	vector<Coordinate> worldCoords() const;
	Coordinate centerCoordinate() const;

	void normalizeIn(Window window);
	virtual void applyClipping() = 0;		
 	vector<CLIPPED_OBJECT> getClippedObjects();

	void translate(VECTOR deslocation);
	void scaleTo(VECTOR factors);
	void rotate(double radians);
	void rotate(double radians, Coordinate anchor);

private:
	void applyTransformation(SQUARE_MATRIX transfMatrix);
	void positionBasedTransformation(SQUARE_MATRIX targetTransformation, Coordinate coord);

protected:
	string _name;
	GeometricElementType _type;
	vector<Coordinate> _worldCoords;
	vector<Coordinate> _windowCoords;
	vector<CLIPPED_OBJECT> _clippedObjects;
};

#endif /* SRC_GEOMETRICELEMENT_H_ */
