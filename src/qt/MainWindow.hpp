#pragma once
#include <qt/MainWidget.hpp>
#include <simulation/Simulation.hpp>
#include <qmainwindow.h>
#include <qwidget.h>

class MainWindow : public QMainWindow {
	Q_OBJECT
public:

	explicit MainWindow(simulation::Domain& domain, int x, int y, int w, int h, QWidget* parent = nullptr);
	~MainWindow();
private:
	MainWidget* mainWidget_;
	int x_;
	int y_;
	int w_;
	int h_;
	simulation::Domain& domain_;
};