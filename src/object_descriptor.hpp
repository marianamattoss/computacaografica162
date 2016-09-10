#ifndef SRC_OBJ_DESCRIPTOR_H_
#define SRC_OBJ_DESCRIPTOR_H_

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>

#include "geometric_element.hpp"
#include "transformable_element.hpp"
#include "point.hpp"
#include "line.hpp"
#include "polygon.hpp"

using namespace std;

class ObjectDescriptor {
private:
	vector<string> _vertices;
	vector<string> _fileBody;

	vector<string> getTokens(string line);
	Coordinate getVertexCoordinate(vector<string> tokens);
	vector<Coordinate> getObjectCoordinates(vector<string> tokens, vector<Coordinate> vertices);

public:
	ObjectDescriptor();
	virtual ~ObjectDescriptor();

	void store(string name, GeometricElementType type, vector<Coordinate> coords);
	void persist(void);

	vector<GeometricElement*> parse(string filePath);
};

#endif /* SRC_OBJ_DESCRIPTOR_H_ */
