#include <qt/MainWindow.hpp>
#include <qtimer>
#include <qboxlayout.h>

MainWindow::MainWindow(int x, int y, int w, int h, QWidget* parent) : 
	x_(x), y_(y), w_(w), h_(h), QMainWindow(parent) {

	this->setGeometry(x_, y_, w_, h_);
	this->setFixedSize(w_, h_);
	QTimer* timer = new QTimer;
	mainWidget_ = new MainWidget(timer, w_, h_, this);
	this->setContentsMargins(50, 50, 50, 50);
	this->setCentralWidget(mainWidget_);
}

MainWindow::~MainWindow() = default;