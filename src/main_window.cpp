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
	// Set background color
	cairo_set_source_rgb(cr, 1, 1, 1);
	
	// Fill in the background color
	cairo_paint(cr);

	GtkWidget *drawingArea = GTK_WIDGET(gtk_builder_get_object(_definitions, "drawingArea"));
	int xViewMax = gtk_widget_get_allocated_width(drawingArea) - MARGIN;
	int yViewMax = gtk_widget_get_allocated_height(drawingArea) - MARGIN;

	cairo_move_to(cr, MARGIN, MARGIN);
	cairo_line_to(cr, xViewMax, MARGIN);
	cairo_line_to(cr, xViewMax, yViewMax);
	cairo_line_to(cr, MARGIN, yViewMax);
	cairo_line_to(cr, MARGIN, MARGIN);

	cairo_stroke(cr);
}

void MainWindow::drawObjects(cairo_t *cr) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 1);

	vector<DrawableObject> elements = mapToViewport();

	if (elements.size() > 0) {
		for (uint i = 0; i < elements.size(); ++i) {
			drawSingleObject(cr, elements[i]);
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
	
	cairo_stroke(cr);
}

vector<DrawableObject> MainWindow::mapToViewport() {
	vector<GeometricElement*> elements = _world->getObjects();
	vector<DrawableObject> drawableElements;
	vector<Coordinate> newCoordinates;

	GtkWidget *drawingArea = GTK_WIDGET(gtk_builder_get_object(_definitions, "drawingArea"));
	Window window = _world->getWindow();

	int xViewMax = gtk_widget_get_allocated_width(drawingArea) - MARGIN;
	int yViewMax = gtk_widget_get_allocated_height(drawingArea) - MARGIN;

	int x,y;

	for (GeometricElement * element : elements) {

		newCoordinates.clear();

		for (Coordinate coord : element->coords()) {
			x = MARGIN + ( ((coord._x + 1) / 2) * (xViewMax - MARGIN) );
			y = MARGIN + ( (1 - (coord._y + 1 ) / 2 ) * (yViewMax - MARGIN) );

			newCoordinates.push_back(Coordinate(x, y));
		}

		if (!newCoordinates.empty())
			drawableElements.push_back(DrawableObject(newCoordinates, element->type()));

	}

	return drawableElements;
}

void MainWindow::updateRowCount(int newValue) {
	_rowCount = newValue;
}

int MainWindow::rowCount() {
	return _rowCount;
}
