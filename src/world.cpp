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
	for (GeometricElement * object : _displayFile) {
		normalize(object);
	}
}

void World::rotateWindow(double angle) {
	window.rotate( DEG2RAD(angle) );
	normalizeObjects();
}

void World::normalize(GeometricElement* object) {
	object->normalizeIn(window);
}

vector<GeometricElement*> World::getObjects(){
	return _displayFile;
}

GeometricElement* World::getObjectBy(string name) {
	for (GeometricElement * object : _displayFile) {
		if(object->name() == name) {
			return object;
		}
	}
}

void World::translateObject(string name, VECTOR deslocation) {
	GeometricElement * targetObject = getObjectBy(name);
	targetObject->translate(deslocation);
	normalize(targetObject);
}

void World::scaleObject(string name, VECTOR factor) {
	GeometricElement* targetObject = getObjectBy(name);
	targetObject->scaleTo(factor);
	normalize(targetObject);
}

void World::rotateObject(string name, double angle) {
	GeometricElement* targetObject = getObjectBy(name);
	targetObject->rotate( DEG2RAD(angle) );
	normalize(targetObject);
}

void World::rotateObject(string name, double angle, Coordinate anchor) {
	GeometricElement* targetObject = getObjectBy(name);
	targetObject->rotate(DEG2RAD(angle), anchor);
	normalize(targetObject);
}

void World::exportToObj() {
	ObjectDescriptor* exporter = new ObjectDescriptor();

	for ( GeometricElement * object : _displayFile ) {
		exporter->store(object->name(), object->type(),object->worldCoords());
	}

	exporter->persist();
}

vector<string> World::importFromObj(string filePath) {
	ObjectDescriptor* importer = new ObjectDescriptor();
	vector<string> names;

	_displayFile = importer->parse(filePath);

	for (GeometricElement * object : _displayFile) {
		names.push_back(object->name());
	}

	normalizeObjects();

	return names;
}
