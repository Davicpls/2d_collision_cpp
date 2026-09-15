#include <qt/MainWidget.hpp>

MainWidget::MainWidget(QTimer* timer, int w, int h, QWidget* parent) 
	: timer_(timer), w_(w), h_(h), QWidget(parent) {}


void MainWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);

}