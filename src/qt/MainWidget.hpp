#pragma once
#include <simulation/Simulation.hpp>
#include <qwidget.h>
#include <qtimer.h>
#include <qpaintevent>
#include <qpainter>

class MainWidget : public QWidget {
	Q_OBJECT
public:
	MainWidget(QTimer* timer, simulation::Domain& domain, int w, int h, QWidget* parent = nullptr);
	void drawScene(QPainter& painter);
signals:
	void ballsPosChanged();
private:
	QTimer* timer_;
	int w_;
	int h_;
	int deltaTime_ = 16;
	simulation::Domain& domain_;

	void drawRect(QPainter& painter);
	void drawBalls(QPainter& painter);
	void updateGrid();
protected:
	void paintEvent(QPaintEvent* event) override;
	QSize sizeHint() const override { return QSize(w_, h_); }
};