#include "plotter.h"

Plotter::Plotter(QWidget *parent)
	: QWidget(parent)
{
    curCoordSysType = Cartesian;
    curFormulaType = SinX;
	minX = 1/*-10.0*/;
	minY = 1/*-10.0*/;
    maxX = 10.0;
    maxY = 10.0;
    numXTicks = 20;
    numYTicks = 20;
	numXGrid = 30;
	numYGrid = 30;
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

void Plotter::calculatePoints()
{
	switch (curFormulaType) {
	case SinX:
		{
			
			break;
		}
	case CosX:
		{
			break;
		}
	case TanX:
		{
			break;
		}
	default:
		break;
	}
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
	for (int i = 0; i <= numXGrid; i++) {
		int x = rect.left() + (i * (rect.width() - 1) / numXGrid);
		painter->drawLine(x, rect.top(), x, rect.bottom());
	}

	for (int j = 0; j <= numYGrid; j++) {
		int y = rect.top() + (j * (rect.height() - 1) / numYGrid);
		painter->drawLine(rect.left(), y, rect.right(), y);
	}

	painter->setClipRect(rect.adjusted(+1,+1,-1,-1));

	DrawAxisType drawAxisType = DrawNone;
	if (minY * maxY < 0 && minX > 0)
		drawAxisType = DrawXAxis;
	else if (minX * maxX < 0 && minY > 0)
		drawAxisType = DrawYAxis;
	else if (minX * maxX < 0 && minY * maxY < 0)
		drawAxisType = DrawXYAxis;
	else if (minX > 0 && minY > 0)
		drawAxisType = DrawLFrameAxis | DrawDFrameAxis;
	else if (maxX < 0 && minY > 0)
		drawAxisType = DrawRFrameAxis | DrawDFrameAxis;
	else if (maxX < 0 && maxY < 0)
		drawAxisType = DrawRFrameAxis | DrawUFrameAxis;
	else if (minX > 0 && maxY < 0)
		drawAxisType = DrawLFrameAxis | DrawUFrameAxis;



	double oX = 0.0;
	double oY = 0.0;
	painter->setPen(Qt::black);
	
	if (drawAxisType & DrawXAxis) {
		oY = rect.bottom() - abs(minY) * (rect.height() - 1) / spany;
		painter->drawLine(rect.left(), oY, rect.right(), oY);
		for (int i = 0; i <= numXTicks; i++) {
			int x = rect.left() + (i * (rect.width() - 1) / numXTicks);
			double label = minX + i * spanx / numXTicks;
			painter->drawLine(x, oY, x, oY - 5);
			if (abs(label) > EPSION)
				painter->drawText(x - 50, 
				oY + 5, 
				100, 
				20, 
				Qt::AlignHCenter | 
				Qt::AlignTop, QString::number(label, 'g', 3));
		}
	}

	if (drawAxisType & DrawYAxis) {
		oX = rect.left() + abs(minX) * (rect.width() - 1) / spanx;
		painter->drawLine(oX, rect.top(), oX, rect.bottom());
		for (int j = 0; j <= numYTicks; j++) {
			int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
			double label = minY + j * spany / numYTicks;
			yVec.push_back(label);
			painter->drawLine(oX, y, oX + 5, y);
			if (abs(label) > EPSION)
				painter->drawText(oX - Margin, 
				rect.bottom() + 40 - y, 
				Margin - 5, 20, Qt::AlignVCenter | 
				Qt::AlignRight, QString::number(label, 'g', 3));
		}
	}

	if (drawAxisType & DrawFrameAxis) {
		painter->setClipRect(0, 0, width(), height());
		
		for (int i = 0; i <= numXTicks; i++) {
			int x = rect.left() + (i * (rect.width() - 1) / numXTicks);
			double label = minX + i * spanx / numXTicks;
			painter->drawText(x - 50, 
				rect.bottom() + 5, 
				100, 
				20, 
				Qt::AlignHCenter | 
				Qt::AlignTop, 
				QString::number(label, 'g', 3));
		}

		for (int j = 0; j <= numYTicks; j++) {
			int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
			double label = minY + j * spany / numYTicks;
			painter->drawText(rect.left() - Margin, 
				rect.bottom() + 40 - y, 
				Margin - 5, 
				20, 
				Qt::AlignVCenter | 
				Qt::AlignRight, QString::number(label, 'g', 3));
		}
	}

	if (minX * maxX <= EPSION && minY * maxY <= EPSION) {
		painter->drawText(oX - Margin, 
			oY, 
			Margin - 5, 
			20, 
			Qt::AlignVCenter | 
			Qt::AlignRight, QString::number(0) );
	}
}

void Plotter::drawCurves(QPainter *painter)
{
	static const QColor colorForIds[6] = {
		Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
	};

	QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
	if (!rect.isValid()) {
		return ;
	}

	for (int i = 0; i <= numXTicks; i++) {

	}
}

QSize Plotter::minimumSizeHint() const
{
	return QSize(500, 500);
}

