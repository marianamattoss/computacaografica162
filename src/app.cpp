#include "app.hpp"

void app_init(App *app) {
	GError *err = NULL;

	app->definitions = gtk_builder_new();

	gtk_builder_add_from_file(app->definitions, UI_DEFINITIONS_FILE, &err);

	if(err != NULL) {
		g_printerr("Error while loading app definitions file: %s\n", err->message);
		g_error_free(err);
		gtk_main_quit();
	}

	gtk_builder_connect_signals(app->definitions, app);

	app->objects = gtk_builder_get_objects(app->definitions);
	app->world = new World();
	app->mainWindow = new MainWindow(app->definitions, app->world);

	GET_UI_ELEMENT(GtkWidget, mainWindow);

	gtk_widget_show_all(mainWindow);
	gtk_main();
}

GObject *app_get_ui_element(App *app, const gchar *name) {
	return gtk_builder_get_object(app->definitions, name);
}
