#include "world.hpp"

World::World() : window(550, 550) {
	// TODO Auto-generated constructor stub
}

World::~World() {
	// TODO Auto-generated destructor stub
}

void World::addPoint(string name, Coordinate coord) {
	Point * point = new Point(name, coord);
	normalize(point);

	_displayFile.push_back(point);
}

void World::addLine(string name, Coordinate begin, Coordinate end) {
	Line * line = new Line(name, begin, end);
	normalize(line);

	_displayFile.push_back(line);
}

void World::addPolygon(string name, vector<Coordinate> coords) {
	Polygon * polygon = new Polygon(name, coords);
	normalize(polygon);

	_displayFile.push_back(polygon);
}

Window World::getWindow() {
	return window;
}

void World::moveWindow(VECTOR step) {
	window.move(step);
	normalizeObjects();
}

void World::zoomWindow(int step) {
	window.zoom(step);
	normalizeObjects();
}

void World::normalizeObjects() {
	for (GeometricObject * object : _displayFile) {
		normalize(object);
	}
}

void World::normalize(GeometricObject* object) {
	object->normalizeIn(window);
}

vector<GeometricObject*> World::getObjects(){
	return _displayFile;
}

GeometricObject* World::getObjectBy(string name) {
	for (GeometricObject * object : _displayFile) {
		if(object->name() == name) {
			return object;
		}
	}
}

void World::translateObject(string name, VECTOR deslocation) {
	GeometricObject * targetObject = getObjectBy(name);
	targetObject->translate(deslocation);
	normalize(targetObject);
}

void World::scaleObject(string name, VECTOR factor) {
	GeometricObject* targetObject = getObjectBy(name);
	targetObject->scaleTo(factor);
	normalize(targetObject);
}

void World::rotateObject(string name, double angle) {
	GeometricObject* targetObject = getObjectBy(name);
	targetObject->rotate( DEG2RAD(angle) );
	normalize(targetObject);
}

void World::rotateObject(string name, double angle, Coordinate anchor) {
	GeometricObject* targetObject = getObjectBy(name);
	targetObject->rotate(DEG2RAD(angle), anchor);
	normalize(targetObject);
}
