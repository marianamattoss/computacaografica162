#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include <string>

#include "b_spline.hpp"
#include "geometric_object.hpp"
#include "point.hpp"
#include "line.hpp"
#include "polygon.hpp"
#include "cubic_bezier.h"
#include "window.hpp"
#include "object_descriptor.hpp"
#include "clipper.hpp"

using namespace std;

class World {
private:
	vector<GeometricObject*> _displayFile;
	Window window;

	void normalizeObjects();
	void normalize(GeometricObject* object);

	GeometricObject* getObjectBy(string name);

public:
	World();
	virtual ~World();

	void addPoint(string name, Coordinate coord);
	void addLine(string name, Coordinate begin, Coordinate end);
	void addPolygon(string name, vector<Coordinate> coords, bool filled);
	void addBezier(string name, vector<Coordinate> controlPoints, double step);
	void addBSpline(string name, vector<Coordinate> controlPoints, double step);

	Window getWindow();

	void moveWindow(VECTOR step);
	void zoomWindow(int step);
	void rotateWindow(double angle);

	vector<GeometricObject*> getObjects();

	void translateObject(string name, VECTOR deslocation);
	void scaleObject(string name, VECTOR factor);
	void rotateObject(string name, double angle);
	void rotateObject(string name, double angle, Coordinate anchor);

	void exportToObj();
	vector<string> importFromObj(string filePath);
};

#endif /* SRC_WORLD_H_ */
