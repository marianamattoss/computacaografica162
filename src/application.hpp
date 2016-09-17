#ifndef __APPLICATION__
#define __APPLICATION__

#include <gtk/gtk.h>
#include "main_window.hpp"
#include "world.hpp"
#include "clipper.hpp"

#define UI_DEFINITIONS_FILE "ui.glade"
#define GET_UI_ELEMENT(TYPE, ELEMENT)   TYPE *ELEMENT = (TYPE *) app_get_ui_element(app, #ELEMENT);

typedef struct application_
{
	GtkBuilder *definitions;
	GSList *objects;
	MainWindow *mainWindow;
	World *world;
} Application;

void app_init (Application *);
GObject * app_get_ui_element (Application * , const gchar * );

#endif
