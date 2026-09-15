#pragma once
#include <qwidget.h>
#include <qtimer.h>
#include <qpaintevent>

class MainWidget : public QWidget {
	Q_OBJECT
public:
	MainWidget(QTimer* timer, int w, int h, QWidget* parent = nullptr);
private:
	QTimer* timer_;
	int w_;
	int h_;
protected:
	void paintEvent(QPaintEvent* event) override;
	QSize sizeHint() const override { return QSize(w_, h_); }
};