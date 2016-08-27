#include "object_descriptor.hpp"

ObjectDescriptor::ObjectDescriptor() {}
ObjectDescriptor::~ObjectDescriptor() {}

void ObjectDescriptor::store(string name,  GeometricObjectType type, vector<Coordinate> coords) {
	size_t initialVertexPosition = _vertices.size() + 1;
	string object;
	string vertex;

	setlocale(LC_ALL, "en_US.utf8");

	for(Coordinate coord : coords) {
		vertex = "v " + to_string(coord._x) + ", " + to_string(coord._y) + ", 0.0";
		_vertices.push_back(vertex);
	}

	if(type == GeometricObjectType::point) {
		object = "p " + to_string(initialVertexPosition);
	} else {
		object = "l " + to_string(initialVertexPosition);

		unsigned int position = static_cast<unsigned int>(initialVertexPosition) + 1;
		unsigned int positionsLimit = static_cast<unsigned int>(_vertices.size());

		for(position; position <= positionsLimit; position++) {
			object += ", " + to_string(position);
		}

		if(type == GeometricObjectType::polygon) {
			object += ", " + to_string(initialVertexPosition);
		}
	}

	_fileBody.push_back("o " + name);
	_fileBody.push_back(object);
}

void ObjectDescriptor::persist() {
	ofstream file;
	file.open("export.obj", ofstream::trunc);

	for(string vertex : _vertices) {
		file << vertex << endl;
	}

	for(string declaration : _fileBody) {
		file << declaration << endl;
	}

	file.close();
}

vector<GeometricObject*> ObjectDescriptor::parse(string filePath) {
	ifstream file(filePath);
	string objectName;
	vector<Coordinate> vertices;
	vector<GeometricObject*> newDisplayFile;

	for(std::string line; getline(file, line);) {
		vector<string> tokens = getTokens(line);

		if(tokens[0] == "v") {
			vertices.push_back(getVertexCoordinate(tokens));
		} else if(tokens[0] == "o") {
			objectName = tokens[1];
		} else {
			vector<Coordinate> objectCoordinates = getObjectCoordinates(tokens, vertices);

			if(tokens[0] == "p") {
				newDisplayFile.push_back(new Point(objectName, objectCoordinates[0]));
			} else if(objectCoordinates.size() == 2) {
				newDisplayFile.push_back(new Line(objectName, objectCoordinates[0], objectCoordinates[1]));
			} else {
				newDisplayFile.push_back(new Polygon(objectName, objectCoordinates, false));
			}
		}
	}

	file.close();

	return newDisplayFile;
}

vector<string> ObjectDescriptor::getTokens(string line) { 
	string buffer;
	stringstream stream(line);

	vector<string> tokens;

	while (stream >> buffer) {
		tokens.push_back(buffer);
	}

	return tokens;
}

Coordinate ObjectDescriptor::getVertexCoordinate(vector<string> tokens) {
	vector<double> coords;

	for(unsigned int i = 1; i < tokens.size(); i++) { 
		string coord = tokens[i];
		coord.erase(remove(coord.begin(), coord.end(), ','), coord.end());

		coords.push_back(stod(coord));
	}

	return Coordinate(coords[0], coords[1]);
}

vector<Coordinate> ObjectDescriptor::getObjectCoordinates(vector<string> tokens, vector<Coordinate> vertices) {
	vector<Coordinate> coords;

	for(unsigned int i = 1; i < tokens.size(); i++) { 
		string coordinateIndex = tokens[i];
		coordinateIndex.erase(remove(coordinateIndex.begin(), coordinateIndex.end(), ','), coordinateIndex.end());

		coords.push_back(vertices[stoi(coordinateIndex) - 1]);
	}

	return coords;
}
