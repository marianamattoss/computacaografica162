#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include <string>

#include "geometric_element.hpp"
#include "point.hpp"
#include "line.hpp"
#include "polygon.hpp"
#include "window.hpp"

using namespace std;

class World {
private:
	vector<GeometricElement*> _displayFile;
	Window window;

	void normalizeObjects();
	void normalize(GeometricElement* object);

	GeometricElement* getObjectBy(string name);

public:
	World();
	virtual ~World();

	void addPoint(string name, Coordinate coord);
	void addLine(string name, Coordinate begin, Coordinate end);
	void addPolygon(string name, vector<Coordinate> coords);

	Window getWindow();

	void moveWindow(VECTOR step);
	void zoomWindow(int step);

	vector<GeometricElement*> getObjects();

	void translateObject(string name, VECTOR deslocation);
	void scaleObject(string name, VECTOR factor);
	void rotateObject(string name, double angle);
	void rotateObject(string name, double angle, Coordinate anchor);
};

#endif /* SRC_WORLD_H_ */
