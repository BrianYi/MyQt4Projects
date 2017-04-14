#include "plotter.h"

Plotter::Plotter(QWidget *parent)
	: QWidget(parent)
{
    curCoordSysType = Cartesian;
    curFormulaType = SinX;
	minX = -10.0;
	minY = -10.0;
    maxX = 10.0;
    maxY = 10.0;
    numXTicks = 20;
    numYTicks = 20;
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFocusPolicy(Qt::StrongFocus);
}

void Plotter::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	drawGrid(&painter);
}

void Plotter::drawGrid(QPainter *painter)
{
	QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
	if (!rect.isValid()) {
		return ;
	}

	QPen light = palette().dark().color().light();
	double spanx = spanX();
	double spany = spanY();
	painter->setPen(light);
	for (int i = 0; i <= numXTicks; i++) {
		int x = rect.left() + (i * (rect.width() - 1) / numXTicks);
		double label = minX + i * spanx / numXTicks;
		painter->drawLine(x, rect.top(), x, rect.bottom());
		/*painter->drawText(x - 50, rect.bottom() + 5, 100, 20, Qt::AlignHCenter | Qt::AlignTop, QString::number(label, 'g', 3));*/
	}

	for (int j = 0; j <= numYTicks; j++) {
		int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
		double label = minY + j * spany / numYTicks;
		painter->drawLine(rect.left(), y, rect.right(), y);
		/*painter->drawText(rect.left() - Margin, rect.bottom() + 40 - y, Margin - 5, 20, Qt::AlignVCenter | Qt::AlignRight, QString::number(label,'g',3));*/
	}

	painter->setClipRect(rect.adjusted(+1,+1,-1,-1));
	double oX = 0.0;
	double oY = 0.0;
	painter->setPen(Qt::black);
	if (minX * maxX < 0) {
		oX = rect.left() + abs(minX) * (rect.width() - 1) / spanx;
		painter->drawLine(oX, rect.top(), oX, rect.bottom());
		for (int j = 1; j < numYTicks; j++) {
			int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
			double label = minY + j * spany / numYTicks;
			painter->drawLine(oX, y, oX + 5, y);
			if (abs(label) > EPSION)
				painter->drawText(oX - Margin, rect.bottom() + 40 - y, Margin - 5, 20, Qt::AlignVCenter | Qt::AlignRight, QString::number(label, 'g', 3));
		}
	}

	if (minY * maxY < 0) {
		oY = rect.bottom() - abs(minY) * (rect.height() - 1) / spany;
		painter->drawLine(rect.left(), oY, rect.right(), oY);
		for (int i = 1; i < numXTicks; i++) {
			int x = rect.left() + (i * (rect.width() - 1) / numXTicks);
			double label = minX + i * spanx / numXTicks;
			painter->drawLine(x, oY, x, oY - 5);
			if (abs(label) > EPSION)
				painter->drawText(x - 50, oY + 5, 100, 20, Qt::AlignHCenter | Qt::AlignTop, QString::number(label, 'g', 3));
		}
	}

	if (minX * maxX <= EPSION && minY * maxY <= EPSION) {
		painter->drawText(oX - Margin, oY, Margin - 5, 20, Qt::AlignVCenter | Qt::AlignRight, QString::number(0) );
	}
}

void Plotter::drawCurves(QPainter *painter)
{

}

QSize Plotter::minimumSizeHint() const
{
	return QSize(500, 500);
}

