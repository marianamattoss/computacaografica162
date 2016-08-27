#include "app.hpp"

int main(int argc, char *argv[])
{
	App *app;
	app = (App *) g_new (App, 1);

	gtk_init(&argc, &argv);
	app_init(app);

	return 0;
}
