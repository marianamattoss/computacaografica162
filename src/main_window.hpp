#ifndef VIEW_MAINWINDOW_H_
#define VIEW_MAINWINDOW_H_

#include <gtk/gtk.h>

#include "geometric_element.hpp"
#include "world.hpp"
#include "window.hpp"
#include "matrix.hpp"

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

struct DrawableObject {

	DrawableObject(vector<Coordinate> coords, GeometricElementType type) :
		_coords(coords), _type(type) {};

private:
	vector<Coordinate> _coords;
	GeometricElementType _type;

public:

	vector<Coordinate> coords() {
		return _coords;
	}

	GeometricElementType type() { return _type; }
};

class MainWindow {
private:
	GtkBuilder *_definitions;
	World *_world;

	const int MARGIN = 10;
	int _rowCount = 8;

public:

	MainWindow(GtkBuilder *definitions, World *world);
	virtual ~MainWindow();

	void updateViewport();
	void updateRowCount(int newValue);

	void drawViewport(cairo_t *cr);
	void drawObjects(cairo_t *cr);
	void drawSingleObject(cairo_t *cr, DrawableObject object);

	vector<Coordinate> readCoordFrom(GtkTreeModel *treeModel);
	int rowCount();

	vector<DrawableObject> mapToViewport();
};

#endif /* VIEW_MAINWINDOW_H_ */
