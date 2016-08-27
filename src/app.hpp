#ifndef __APP__
#define __APP__

#include <gtk/gtk.h>
#include "main_window.hpp"
#include "world.hpp"

#define UI_DEFINITIONS_FILE "ui.glade"
#define GET_UI_ELEMENT(TYPE, ELEMENT)   TYPE *ELEMENT = (TYPE *) app_get_ui_element(app, #ELEMENT);

typedef struct app_
{
	GtkBuilder *definitions;
	GSList *objects;
	MainWindow *mainWindow;
	World *world;
} App;

void app_init (App *);
GObject * app_get_ui_element (App * , const gchar * );

#endif
