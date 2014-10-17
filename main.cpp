
#include "MainWindow.h"
#include <QtWidgets/QApplication>

using namespace Pirogronian;

int main(int argc, char **argv) {
	
	QApplication app(argc, argv);
	
	app.setOrganizationName("Pirogronian");
	app.setApplicationName("Elastic Body Simulator");
	
	MainWindow win;
	
	win.show();
	
	return app.exec();
	
}
