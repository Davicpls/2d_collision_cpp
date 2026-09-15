#include <qt/MainWindow.hpp>
#include <qapplication.h>

int main(int argc, char* argv[]) {
	int x = 250;
	int y = 50;
	int w = 1450;
	int h = 950;

	QApplication app(argc, argv);
	MainWindow window(x, y, w, h);
	window.show();

	return app.exec();
}