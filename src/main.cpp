#include <qt/MainWindow.hpp>
#include <simulation/Simulation.hpp>
#include <qapplication.h>

int main(int argc, char* argv[]) {
	int x = 250;
	int y = 50;
	int w = 1450;
	int h = 950;

	QApplication app(argc, argv);
	simulation::SpatialHashGrid spatialGrid(2);
	simulation::Domain domain(spatialGrid);
	domain.populateGrid();
	MainWindow window(domain, x, y, w, h);
	window.show();

	return app.exec();
}