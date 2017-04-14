#include "plotter.h"

Plotter::Plotter(QWidget *parent)
	: QWidget(parent)
{
    curCoordSysType = Cartesian;
    curFormulaType = SinX;
    curCurveSmoothLevelType = SmoothLevel0;
	minX = -10/*-10.0*/;
	minY = -10/*-10.0*/;
    maxX = 10.0;
    maxY = 10.0;
    numXTicks = 20;
    numYTicks = 20;
	numXGrid = 40;
	numYGrid = 40;
    polyline = new QPolygonF;
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFocusPolicy(Qt::StrongFocus);
}

void Plotter::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	drawGrid(&painter);
    drawCurves(&painter);
}

void Plotter::calculatePoints()
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    double spanx = spanX();
    double spany = spanY();
    if (!polyline->isEmpty()) {
        polyline->clear();
    }

    for (int i = 0; i <= curCurveSmoothLevelType; i++) {
        double valX = minX + i * spanx / curCurveSmoothLevelType;
        double valY = calculate(valX);
        double dx = valX - minX;
        double dy = valY - minY;
        double x = rect.left() + (dx * (rect.width() - 1) / spanx);
        double y = rect.bottom() - (dy * (rect.height() - 1) / spany);
        polyline->push_back(QPointF(x, y));
    }

// 	switch (curFormulaType) {
// 	case SinX:
// 		{
// 			for (int i = 0; i <= numXTicks; i++) {
//                 double valX = minX + i * spanx / numXTicks;
//                 double valY = sinf(valX);
//                 double dx = valX - minX;
//                 double dy = valY - minY;
//                 double x = rect.left() + (dx * (rect.width() - 1) / spanx);
//                 double y = rect.bottom() - (dy * (rect.height() - 1) / spany);
//                 polyline->push_back(QPointF(x, y));
// 			}
// 			break;
// 		}
// 	case CosX:
// 		{
//             for (int i = 0; i <= numXTicks; i++) {
//                 double valX = minX + i * spanx / numXTicks;
//                 double valY = cosf(valX);
//                 double dx = valX - minX;
//                 double dy = valY - minY;
//                 double x = rect.left() + (dx * (rect.width() - 1) / spanx);
//                 double y = rect.bottom() - (dy * (rect.height() - 1) / spany);
//                 polyline->push_back(QPointF(x, y));
//             }
// 			break;
// 		}
// 	case TanX:
// 		{
// 			break;
// 		}
// 	default:
// 		break;
// 	}
}


double Plotter::calculate(const double &val)
{
    switch (curFormulaType) {
    case SinX:
        return sinf(val);
    case CosX:
        return cosf(val);
    case TanX:
        return tanf(val);
    default:
        break;
    }
    return 0.0;
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

	int drawAxisType = DrawNone;
	if (minY * maxY < 0/* && (minX > 0 || abs(minX) < EPSION)*/)
		drawAxisType |= DrawXAxis;
	if (minX * maxX < 0/* && (minY > 0 || abs(minY) < EPSION)*/)
		drawAxisType |= DrawYAxis;
// 	if (minX * maxX < 0 && minY * maxY < 0)
// 		drawAxisType |= DrawXYAxis;
    if (minX > 0 || abs(minX) < EPSION)
        drawAxisType |= DrawLFrameAxis;
    if (maxX < 0 || abs(maxX) < EPSION)
        drawAxisType |= DrawRFrameAxis;
    if (minY > 0 || abs(minY) < EPSION)
        drawAxisType |= DrawDFrameAxis;
    if (maxY < 0 || abs(maxY) < EPSION)
        drawAxisType |= DrawUFrameAxis;
// 	else if ((minX > 0 || abs(minX) < EPSION) && (minY > 0 || abs(minY) < EPSION))
// 		drawAxisType = DrawLFrameAxis | DrawDFrameAxis;
// 	else if ((maxX < 0 || abs(maxX) < EPSION) && (minY > 0 || abs(minY) < EPSION))
// 		drawAxisType = DrawRFrameAxis | DrawDFrameAxis;
// 	else if ((maxX < 0 || abs(maxX) < EPSION) && (maxY < 0 || abs(maxY) < EPSION))
// 		drawAxisType = DrawRFrameAxis | DrawUFrameAxis;
// 	else if ((minX > 0 || abs(minX) < EPSION) && (maxY < 0 || abs(maxY) < EPSION))
// 		drawAxisType = DrawLFrameAxis | DrawUFrameAxis;



	double oX = rect.left() + abs(minX) * (rect.width() - 1) / spanx;
	double oY = rect.bottom() - abs(minY) * (rect.height() - 1) / spany;
	painter->setPen(Qt::black);
	
	if (drawAxisType & DrawXAxis) {
		//oY = rect.bottom() - abs(minY) * (rect.height() - 1) / spany;
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
		//oX = rect.left() + abs(minX) * (rect.width() - 1) / spanx;
		painter->drawLine(oX, rect.top(), oX, rect.bottom());
		for (int j = 0; j <= numYTicks; j++) {
			int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
			double label = minY + j * spany / numYTicks;
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

        if (drawAxisType & DrawDFrameAxis) {
            painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());		
            for (int i = 0; i <= numXTicks; i++) {
                int x = rect.left() + (i * (rect.width() - 1) / numXTicks);
                double label = minX + i * spanx / numXTicks;
                painter->drawLine(x, rect.bottom(), x, rect.bottom() - 5);
                if (abs(label) > EPSION)
                    painter->drawText(x - 50, 
                    rect.bottom() + 5, 
                    100, 
                    20, 
                    Qt::AlignHCenter | 
                    Qt::AlignTop, 
                    QString::number(label, 'g', 3));
            }
        }

        if (drawAxisType & DrawUFrameAxis) {
            painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            for (int i = 0; i <= numXTicks; i++) {
                int x = rect.left() + (i * (rect.width() - 1) / numXTicks);
                double label = minX + i * spanx / numXTicks;
                painter->drawLine(x, rect.top(), x, rect.top() + 5);
                if (abs(label) > EPSION)
                    painter->drawText(x - 50, 
                    rect.top() - 15, 
                    100, 
                    20, 
                    Qt::AlignHCenter | 
                    Qt::AlignTop, 
                    QString::number(label, 'g', 3)); 
            }
        }

        if (drawAxisType & DrawLFrameAxis) {
            painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            for (int j = 0; j <= numYTicks; j++) {
                int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
                double label = minY + j * spany / numYTicks;
                painter->drawLine(rect.left(), y, rect.left() + 5, y);
                if (abs(label) > EPSION)
                    painter->drawText(rect.left() - Margin, 
                    rect.bottom() + 40 - y, 
                    Margin - 5, 
                    20, 
                    Qt::AlignVCenter | 
                    Qt::AlignRight, QString::number(label, 'g', 3));
            }
        }

        if (drawAxisType & DrawRFrameAxis) {
            painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
            for (int j = 0; j <= numYTicks; j++) {
                int y = rect.top() + (j * (rect.height() - 1) / numYTicks);
                double label = minY + j * spany / numYTicks;
                painter->drawLine(rect.right(), y, rect.right() - 5, y);
                if (abs(label) > EPSION)
                    painter->drawText(rect.right() - 10, 
                    rect.bottom() + 40 - y, 
                    Margin - 5, 
                    20, 
                    Qt::AlignVCenter | 
                    Qt::AlignRight, QString::number(label, 'g', 3));
            }
        }
    }
	if ((minX * maxX < 0 || abs(minX * maxX) < EPSION) && 
        (minY * maxY < 0 || abs(minY * maxY) < EPSION)) {
		painter->drawText(oX - Margin, 
			oY - 10, 
			100, 
			20, 
			Qt::AlignVCenter | 
			Qt::AlignHCenter, QString::number(0) );
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

	calculatePoints();

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
    painter->setPen(colorForIds[curFormulaType % 6]);
    painter->drawPolyline(*polyline);
}

QSize Plotter::minimumSizeHint() const
{
	return QSize(500, 500);
}

void Plotter::setMinX(const QString &strMinX)
{
    bool ok;
    minX = strMinX.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setMaxX(const QString &strMaxX)
{
    bool ok;
    maxX = strMaxX.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setMinY(const QString &strMinY)
{
    bool ok;
    minY = strMinY.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setMaxY(const QString &strMaxY)
{
    bool ok;
    maxY = strMaxY.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setNumXTicks(const QString &strNumXTicks)
{
    bool ok;
    numXTicks = strNumXTicks.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setNumYTicks(const QString &strNumYTicks)
{
    bool ok;
    numYTicks = strNumYTicks.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setNumXGrid(const QString &strNumXGrid)
{
    bool ok;
    numXGrid = strNumXGrid.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setNumYGrid(const QString &strNumYGrid)
{
    bool ok;
    numYGrid = strNumYGrid.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Warning",  QString(__FILE__) + QString(__LINE__));
        return ;
    }
    update();
}

void Plotter::setCoordSysType(int coordSysType)
{
    curCoordSysType = coordSysType;
    update();
}

void Plotter::setFormulaType(int formulaType)
{
    curFormulaType = formulaType;
    update();
}

void Plotter::setCurveSmoothLevelType(int curveSmoothLvlType)
{
    curCurveSmoothLevelType = curveSmoothLvlType;
    update();
}


