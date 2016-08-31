#include "application.hpp"

int main(int argc, char *argv[])
{
	Application *application;
	application = (Application *) g_new (Application, 1);

	gtk_init(&argc, &argv);
	app_init(application);

	return 0;
}
