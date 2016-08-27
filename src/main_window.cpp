#include <cstdlib>
#include <cstring>
#include "main_window.hpp"

MainWindow::MainWindow(GtkBuilder *definitions, World *world) {
	_definitions = definitions;
	_world = world;
}

MainWindow::~MainWindow() {}

void MainWindow::updateViewport() {
	GtkWidget *drawingArea = GTK_WIDGET(gtk_builder_get_object(_definitions, "drawingArea"));
	gtk_widget_queue_draw(drawingArea);
}

vector<Coordinate> MainWindow::readCoordFrom(GtkTreeModel *treeModel) {
	GtkTreeIter valuesIterator;
	vector<Coordinate> coords;
	gdouble x_axis_value, y_axis_value;
	bool valid;

	valid = gtk_tree_model_get_iter_first(treeModel, &valuesIterator);

	while(valid) {
		gtk_tree_model_get(treeModel, &valuesIterator, X_AXIS, &x_axis_value, Y_AXIS, &y_axis_value, -1);
		coords.push_back(Coordinate(x_axis_value, y_axis_value));

		valid = gtk_tree_model_iter_next(treeModel, &valuesIterator);
	}
	
	return coords;
}

void MainWindow::drawViewport(cairo_t *cr) {
	/* Set color for background */
	cairo_set_source_rgb(cr, 1, 1, 1);
	/* fill in the background color*/
	cairo_paint(cr);

	cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_set_line_width(cr, 1);

	GtkWidget *drawingArea = GTK_WIDGET(gtk_builder_get_object(_definitions, "drawingArea"));
	int Xvmax = gtk_widget_get_allocated_width(drawingArea) - MARGIN;
	int Yvmax = gtk_widget_get_allocated_height(drawingArea) - MARGIN;

	cairo_move_to(cr, MARGIN, MARGIN);
	cairo_line_to(cr, Xvmax, MARGIN);
	cairo_line_to(cr, Xvmax, Yvmax);
	cairo_line_to(cr, MARGIN, Yvmax);
	cairo_line_to(cr, MARGIN, MARGIN);

	cairo_stroke(cr);
}

void MainWindow::drawObjects(cairo_t *cr) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 1);

	vector<DrawableObject> objects = mapToViewport();

	if (objects.size() > 0) {
		for (uint i = 0; i < objects.size(); ++i) {
			drawSingleObject(cr, objects[i]);
		}
	}
}

void MainWindow::drawSingleObject(cairo_t *cr, DrawableObject object) {
	vector<Coordinate> coords = object.coords();

	cairo_move_to(cr, coords[0]._x, coords[0]._y);
	cairo_line_to(cr, coords.front()._x, coords.front()._y);

	for (uint i = 1; i < coords.size(); i++) {
		cairo_line_to(cr, coords[i]._x, coords[i]._y);
	}

	if (coords.size() == 1)
		cairo_line_to(cr, coords[0]._x + 1, coords[0]._y + 1);
	else if (object.type() == polygon)
		cairo_line_to(cr, coords.front()._x, coords.front()._y);

//	if(object.filled())
//		cairo_fill(cr);
	
	cairo_stroke(cr);
}

vector<DrawableObject> MainWindow::mapToViewport() {
	vector<GeometricObject*> objects = _world->getObjects();
	vector<DrawableObject> drawableObjects;
	vector<Coordinate> newcoords, clippedCoords;

	GtkWidget *drawingArea = GTK_WIDGET(gtk_builder_get_object(_definitions, "drawingArea"));
	Window window = _world->getWindow();

	int Xvmax = gtk_widget_get_allocated_width(drawingArea) - MARGIN;
	int Yvmax = gtk_widget_get_allocated_height(drawingArea) - MARGIN;

	int x,y;
	bool shouldFill;

	for (GeometricObject * object : objects) {
		object->applyClipping();

//		if (clippedCoords.empty())
//			continue;

		shouldFill = (object->type() == GeometricObjectType::polygon) && object->filled();

		for (vector<Coordinate> clippedCoords : object->getClippedObjects()) {
			newcoords.clear();

			for (Coordinate coord : clippedCoords) {
				x = MARGIN + ( ((coord._x + 1) / 2) * (Xvmax - MARGIN) );
				y = MARGIN + ( (1 - (coord._y + 1 ) / 2 ) * (Yvmax - MARGIN) );

				newcoords.push_back(Coordinate(x, y));
			}

			if (!newcoords.empty())
				drawableObjects.push_back(DrawableObject(shouldFill ,newcoords, object->type()));
		}

	}

	return drawableObjects;
}

void MainWindow::updateRowCount(int newValue) {
	_rowCount = newValue;
}

int MainWindow::rowCount() {
	return _rowCount;
}
