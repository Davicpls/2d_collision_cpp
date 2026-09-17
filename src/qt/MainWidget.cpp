#include <qt/MainWidget.hpp>
#include <qpoint>

constexpr float SCALE = 2.0f;

MainWidget::MainWidget(QTimer* timer, simulation::Domain& domain, int w, int h, QWidget* parent)
	: domain_(domain), timer_(timer), w_(w), h_(h), QWidget(parent) {
	updateGrid();
	timer_->start(deltaTime_);

}

void MainWidget::drawScene(QPainter& painter) {
	drawRect(painter);
	drawBalls(painter);
}

void MainWidget::drawRect(QPainter& painter) {
	QRect rect(0, 0, w_, h_);
	painter.fillRect(rect, Qt::gray);
}

void MainWidget::drawBalls(QPainter& painter) {
	painter.setRenderHint(QPainter::Antialiasing, true);

	/*int count = 1;*/
/*	qDebug() << "max w: " << w_ / 16 << "\n";
	qDebug() << "max h: " << h_ / 16 << "\n";*/
	for (const auto& entity : domain_.entities()) {
	/*	qDebug() << "BALL: " << count << "\n";*/
/*		qDebug() << "cell x: " << entity->cell.x << "\n";
		qDebug() << "cell y: " << entity->cell.y << "\n";*/
/*		qDebug() << "pos x: " << entity->position.x << "\n";
		qDebug() << "pos y: " << entity->position.y << "\n";*/
/*		qDebug() << "radius: " << entity->radius << "\n";*/
/*		qDebug() << "vel x: " << entity->velocity.x << "\n";
		qDebug() << "vel y: " << entity->velocity.y << "\n";*/
		if (entity->radius == 0.5f) {
			painter.setBrush(QBrush(Qt::blue));
		}
		else if (entity->radius == 2.0f){
			painter.setBrush(QBrush(Qt::red));
		}
		else {
			painter.setBrush(QBrush(Qt::darkGreen));
		}
		painter.drawEllipse(QPointF(entity->position.x * SCALE, entity->position.y * SCALE), entity->radius * SCALE, entity->radius * SCALE);
		/*++count;*/
	}
}

void MainWidget::updateGrid() {
	connect(timer_, &QTimer::timeout, this, [this]() {
		domain_.updateCells(deltaTime_, w_ / 2.0f, h_ / 2.0f);
		emit ballsPosChanged();
		update();
		});
}


void MainWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);

	QPainter painter(this);

	drawScene(painter);

}