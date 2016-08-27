#include "app.hpp"
#include "clipper.hpp"

extern "C" {
	// General -------------------------------------------------------------------------------

	void close_app_handler(GtkWidget *widget, App *app) {
		gtk_main_quit();
	}

	gboolean draw_handler(GtkWidget *widget, cairo_t *cr, App *app) {
		app->mainWindow->drawViewport(cr);
		app->mainWindow->drawObjects(cr);

		return FALSE;
	}

	void add_object_handler(GtkWidget *widget, App *app) {
		GtkWidget *drawingArea = GTK_WIDGET(app_get_ui_element(app, "drawingArea"));
		GtkEntry *objName = GTK_ENTRY(app_get_ui_element(app, "objName"));
		GtkNotebook *objNotebook = GTK_NOTEBOOK(app_get_ui_element(app, "objNotebook"));
		GtkListStore *objStore = GTK_LIST_STORE(app_get_ui_element(app, "objStore"));
		GtkTreeModel *treeModel;
		GtkToggleButton *fillPolygon;
		GtkSpinButton *curveStep;
		vector<Coordinate> coords;
		
		const char* name = gtk_entry_get_text(objName);
		name = (name[0] == '\0') ? "objeto" : name;
		
		int pageIndex = gtk_notebook_get_current_page(GTK_NOTEBOOK(objNotebook));

		switch (pageIndex) {
			case 0:
				// addPoint
				treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(app_get_ui_element(app, "newPointCoordinate")));
				app->world->addPoint(name, app->mainWindow->readCoordFrom(treeModel)[0]);

				break;
			case 1:
				// addLine
				treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(app_get_ui_element(app, "newLineCoordinates")));
				coords = app->mainWindow->readCoordFrom(treeModel);

				app->world->addLine(name, coords[0], coords[1]);

				break;
			case 2:
				// addPolygon
				fillPolygon = GTK_TOGGLE_BUTTON(app_get_ui_element(app, "fillPolygon"));
				treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(app_get_ui_element(app, "newPolygonCoordinates")));
				coords = app->mainWindow->readCoordFrom(treeModel);

				app->world->addPolygon(name, coords, gtk_toggle_button_get_active(fillPolygon));

				break;
			case 3:
				// addBezier
				treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(app_get_ui_element(app, "newCurveCoordinates")));
				coords = app->mainWindow->readCoordFrom(treeModel);
				curveStep = GTK_SPIN_BUTTON(app_get_ui_element(app, "spinCurveStep"));

				app->world->addBezier(name, coords, gtk_spin_button_get_value(curveStep));

				break;
			default:
				// addBSplines
				treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(app_get_ui_element(app, "newBSplineCoordinates")));
				coords = app->mainWindow->readCoordFrom(treeModel);
				curveStep = GTK_SPIN_BUTTON(app_get_ui_element(app, "spinBSplineStep"));

				app->world->addBSpline(name, coords, gtk_spin_button_get_value(curveStep));

				break;
		}

		GtkTreeIter iter;
		gtk_list_store_append(objStore, &iter);
		gtk_list_store_set(objStore, &iter, 0, name, -1);

		gtk_widget_queue_draw(drawingArea);

		GtkWindow *addObjWindow = GTK_WINDOW(app_get_ui_element(app, "addObjectWindow"));
		gtk_window_close(addObjWindow);
	}

	void add_polygon_coord_handler(GtkWidget *widget, App *app) {
		GtkTreeView *treeview = GTK_TREE_VIEW(app_get_ui_element(app, "newPolygonCoordinates"));
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkListStore *store = GTK_LIST_STORE(model);
		GtkTreeIter iter;

		gtk_list_store_append(store, &iter);
		gtk_list_store_set (store, &iter, X_AXIS, 0, Y_AXIS, 0, Z_AXIS, 0, -1);
	}

	void remove_polygon_coord_handler(GtkWidget *widget, App *app) {
		GtkTreeView *treeview = GTK_TREE_VIEW(app_get_ui_element(app, "newPolygonCoordinates"));
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkListStore *store = GTK_LIST_STORE(model);
		GtkTreePath *path;
		GtkTreeIter iter;

		gint rows = gtk_tree_model_iter_n_children(model, NULL);

		if(rows > 3) {
			path = gtk_tree_path_new_from_indices(rows-1, -1);

			gtk_tree_model_get_iter(model, &iter, path);
			gtk_list_store_remove(store, &iter);
			gtk_tree_path_free(path);
		}
	}

	void add_curve_coord_handler(GtkWidget *widget, App *app) {
		GtkTreeView *treeview = GTK_TREE_VIEW(app_get_ui_element(app, "newCurveCoordinates"));
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkListStore *store = GTK_LIST_STORE(model);
		GtkTreeIter iter;

		for(int i=0; i < 3; i++) {
			gtk_list_store_append(store, &iter);
			gtk_list_store_set (store, &iter, X_AXIS, 0, Y_AXIS, 0, Z_AXIS, 0, -1);
		}
	}

	void add_bsplines_coord_handler(GtkWidget *widget, App *app) {
		GtkTreeView *treeview = GTK_TREE_VIEW(app_get_ui_element(app, "newBSplineCoordinates"));
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkListStore *store = GTK_LIST_STORE(model);
		GtkTreeIter iter;

		gtk_list_store_append(store, &iter);
		gtk_list_store_set (store, &iter, X_AXIS, 0, Y_AXIS, 0, Z_AXIS, 0, -1);
	}

	void remove_curve_coord_handler(GtkWidget *widget, App *app) {
		GtkTreeView *treeview = GTK_TREE_VIEW(app_get_ui_element(app, "newCurveCoordinates"));
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkListStore *store = GTK_LIST_STORE(model);
		GtkTreePath *path;
		GtkTreeIter iter;

		gint rows = gtk_tree_model_iter_n_children(model, NULL);

		if(rows > 4) {
			for(int i=0; i<3; i++) {
				path = gtk_tree_path_new_from_indices(rows-1, -1);

				gtk_tree_model_get_iter(model, &iter, path);
				gtk_list_store_remove(store, &iter);
				gtk_tree_path_free(path);

				rows--;
			}
		}
	}

	void remove_bsplines_coord_handler(GtkWidget *widget, App *app) {
		GtkTreeView *treeview = GTK_TREE_VIEW(app_get_ui_element(app, "newCurveCoordinates"));
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkListStore *store = GTK_LIST_STORE(model);
		GtkTreePath *path;
		GtkTreeIter iter;

		gint rows = gtk_tree_model_iter_n_children(model, NULL);

		if(rows > 4) {
			path = gtk_tree_path_new_from_indices(rows-1, -1);

			gtk_tree_model_get_iter(model, &iter, path);
			gtk_list_store_remove(store, &iter);
			gtk_tree_path_free(path);

			rows--;
		}
	}

	void show_add_popup_handler(GtkWidget *widget, App *app) {
		GtkWindow *addObjWindow = GTK_WINDOW(app_get_ui_element(app, "addObjectWindow"));
		gtk_window_present(addObjWindow);
	}

	void close_add_popup_handler(GtkWidget *widget, App *app) {
		GtkWidget *addObjWindow = GTK_WIDGET(app_get_ui_element(app, "addObjectWindow"));
		gtk_widget_hide_on_delete(addObjWindow);
	}

	void close_no_object_handler(GtkWidget *widget, App *app) {
		GtkWidget *noObject = GTK_WIDGET(app_get_ui_element(app, "noObject"));
		gtk_widget_hide_on_delete(noObject);
	}

	void update_cell(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data, int column) {
		GtkTreeView *treeview = GTK_TREE_VIEW(user_data);
		GtkTreeModel *model = gtk_tree_view_get_model(treeview);
		GtkTreeIter iter;
		GValue new_value = G_VALUE_INIT;
		
		gtk_tree_model_get_iter_from_string(model, &iter, path_string);
		
		g_value_init (&new_value, G_TYPE_DOUBLE);
		g_value_set_double (&new_value, atof(new_text));
		
		gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, column, &new_value);		
	}

	void x_axis_cell_updated(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
		update_cell(cell, path_string, new_text, user_data, 0);
	}

	void y_axis_cell_updated(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
		update_cell(cell, path_string, new_text, user_data, 1);
	}

	void z_axis_cell_updated(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
		update_cell(cell, path_string, new_text, user_data, 2);
	}

	// Movements Handler ----------------------------------------------------------------------

	void move_up_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *stepInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "stepSpinBtn"));
		app->world->moveWindow( VECTOR(0, gtk_spin_button_get_value(stepInput)) );

		app->mainWindow->updateViewport();
	}

	void move_right_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *stepInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "stepSpinBtn"));
		app->world->moveWindow( VECTOR( gtk_spin_button_get_value(stepInput), 0 ) );

		app->mainWindow->updateViewport();
	}
	
	void move_down_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *stepInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "stepSpinBtn"));
		app->world->moveWindow( VECTOR( 0, -gtk_spin_button_get_value(stepInput) ) );

		app->mainWindow->updateViewport();

	}

	void move_left_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *stepInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "stepSpinBtn"));
		app->world->moveWindow( VECTOR( -gtk_spin_button_get_value(stepInput), 0 ) );

		app->mainWindow->updateViewport();
	}

	// Zoom Handler ---------------------------------------------------------------------------

	void zoom_in_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *stepInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "stepSpinBtn"));
		app->world->zoomWindow(gtk_spin_button_get_value(stepInput));

		app->mainWindow->updateViewport();
	}
	void zoom_out_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *stepInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "stepSpinBtn"));
		app->world->zoomWindow(-gtk_spin_button_get_value(stepInput));

		app->mainWindow->updateViewport();
	}

	// Transformations ------------------------------------------------------------------------

	void window_rotate_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *angleInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "windowAngleSpin"));
		app->world->rotateWindow( gtk_spin_button_get_value(angleInput) );

		app->mainWindow->updateViewport();
	}

	void translate_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *xAxisInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "xFactorBtn"));
		GtkSpinButton *yAxisInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "yFactorBtn"));
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_get_ui_element(app, "objectsList")));
		GtkTreeModel *model;
		GtkTreeIter iter;

		VECTOR deslocation(gtk_spin_button_get_value(xAxisInput), gtk_spin_button_get_value(yAxisInput));

		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gchar *name;
			
			gtk_tree_model_get(model, &iter, 0, &name, -1);
			app->world->translateObject(string(name), deslocation);

			g_free(name);
		} else {
			GtkWindow *noObject = GTK_WINDOW(app_get_ui_element(app, "noObject"));
			gtk_window_present(noObject);
		}

		app->mainWindow->updateViewport();
	}

	void scale_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *xAxisInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "xFactorBtn"));
		GtkSpinButton *yAxisInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "yFactorBtn"));
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_get_ui_element(app, "objectsList")));
		GtkTreeModel *model;
		GtkTreeIter iter;

		VECTOR factor(gtk_spin_button_get_value(xAxisInput), gtk_spin_button_get_value(yAxisInput));

		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gchar *name;
			
			gtk_tree_model_get(model, &iter, 0, &name, -1);
			app->world->scaleObject(string(name), factor);

			g_free(name);
		} else {
			GtkWindow *noObject = GTK_WINDOW(app_get_ui_element(app, "noObject"));
			gtk_window_present(noObject);
		}

		app->mainWindow->updateViewport();		
	}

	void rotate_handler(GtkWidget *widget, App *app) {
		GtkSpinButton *xAxisInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "xFactorBtn"));
		GtkSpinButton *yAxisInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "yFactorBtn"));
		GtkSpinButton *angleInput = GTK_SPIN_BUTTON(app_get_ui_element(app, "angleBtn"));
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_get_ui_element(app, "objectsList")));
		GtkToggleButton *rotateUsingCentroid = GTK_TOGGLE_BUTTON(app_get_ui_element(app, "rotateUsingCentroid"));
		GtkTreeModel *model;
		GtkTreeIter iter;

		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gchar *name;
			
			gtk_tree_model_get(model, &iter, 0, &name, -1);

			if(gtk_toggle_button_get_active(rotateUsingCentroid)) {
				app->world->rotateObject(string(name), gtk_spin_button_get_value(angleInput));
			} else {
				Coordinate coordinate(gtk_spin_button_get_value(xAxisInput), gtk_spin_button_get_value(yAxisInput));
				app->world->rotateObject(string(name), gtk_spin_button_get_value(angleInput), coordinate);
			}

			g_free(name);
		} else {
			GtkWindow *noObject = GTK_WINDOW(app_get_ui_element(app, "noObject"));
			gtk_window_present(noObject);
		}

		app->mainWindow->updateViewport();				
	}

	void lb_handler(GtkWidget *widget, App *app) {
		Clipper::clippingFunction = Clipper::liangBarsky;
	}

	void cs_handler(GtkWidget *widget, App *app) {
		Clipper::clippingFunction = Clipper::cohenSutherland;
	}

	// Import/Export ----------------------------------------------------------------------------

	void export_handler(GtkWidget *widget, App *app) {
		GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_get_ui_element(app, "textview")));
		GtkTextIter end;

		app->world->exportToObj();

		gtk_text_buffer_get_end_iter(buffer, &end);
		gtk_text_buffer_insert(buffer, &end, "Criado arquivo: export.obj\n", -1);
	}

	void import_handler(GtkWidget *widget, App *app) {
		GtkWidget *dialog;
		GtkListStore *objStore = GTK_LIST_STORE(app_get_ui_element(app, "objStore"));
		GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
		gint response;
		vector<string> objects;
		GtkTreeIter iter;

		dialog = gtk_file_chooser_dialog_new("Importar arquivo", NULL, action, ("_Cancelar"), GTK_RESPONSE_CANCEL, 
											 ("_Importar"), GTK_RESPONSE_ACCEPT, NULL);

		response = gtk_dialog_run(GTK_DIALOG(dialog));
		if (response == GTK_RESPONSE_ACCEPT) {
			GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
			string filePath = gtk_file_chooser_get_filename(chooser);

			objects = app->world->importFromObj(filePath);
		}

		for(string name : objects) {
			gtk_list_store_append(objStore, &iter);
			gtk_list_store_set(objStore, &iter, 0, name.c_str(), -1);
		}

		app->mainWindow->updateViewport();				

		gtk_widget_destroy(dialog);
	}
}
