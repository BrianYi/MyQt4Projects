#include "plotter.h"

Plotter::Plotter(QWidget *parent)
	: QWidget(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    curCoordSysType = Cartesian;
    curFormulaType = SinX;
    curCurveSmoothLevelType = SmoothLevel0;
    curZoom = 0;
    zoomStack.append(PlotterSetting());
    rubberBandIsShown = false;
    pieIsShown = false;
    pieRadius = 5;
    polyline = new QPolygon;
    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":images/zoomin.png"));
    zoomInButton->setToolTip(tr("zoom in"));
    zoomInButton->adjustSize();
    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":images/zoomout.png"));
    zoomOutButton->setToolTip(tr("zoom out"));
	zoomOutButton->adjustSize();
    zoomInButton->hide();
    zoomOutButton->hide();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
}

void Plotter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    drawGrid(&painter, rect);
    drawCurves(&painter, rect);
    drawRubberBandRect(&painter, rect);
    drawPoint(&painter, rect);
}

void Plotter::calculatePoints(const QRect &rect)
{
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    double spanx = plotterSetting.spanX();
    double spany = plotterSetting.spanY();
    if (!curveData.isEmpty() || !polyline->isEmpty()) {
        curveData.clear();
        polyline->clear();
    }

    for (int i = 0; i <= curCurveSmoothLevelType; i++) {
        double valX = plotterSetting.minX + i * spanx / curCurveSmoothLevelType;
        double valY = calculate(valX);
        double dx = valX - plotterSetting.minX;
        double dy = valY - plotterSetting.minY;
        int x = rect.left() + (dx * (rect.width() - 1) / spanx);
        int y = rect.bottom() - (dy * (rect.height() - 1) / spany);
        polyline->push_back(QPoint(x, y));
        curveData.push_back(QPointF(valX, valY));
    }
}


double Plotter::calculate(const double &x)
{
    switch (curFormulaType) {
    case SinX:
        return sinf(x);
    case CosX:
        return cosf(x);
    case TanX:
        return tanf(x);
    default:
        break;
    }
    return 0.0;
}

void Plotter::drawGrid(QPainter *painter, const QRect &rect)
{
    PlotterSetting plotterSetting = zoomStack[curZoom];
	QPen light = palette().dark().color().light();
	double spanx = plotterSetting.spanX();
	double spany = plotterSetting.spanY();
	painter->setPen(light);
	for (int i = 0; i <= plotterSetting.numXGrid; i++) {
		int x = rect.left() + (i * (rect.width() - 1) / plotterSetting.numXGrid);
		painter->drawLine(x, rect.top(), x, rect.bottom());
	}

	for (int j = 0; j <= plotterSetting.numYGrid; j++) {
		int y = rect.top() + (j * (rect.height() - 1) / plotterSetting.numYGrid);
		painter->drawLine(rect.left(), y, rect.right(), y);
	}

	painter->setClipRect(rect.adjusted(+1,+1,-1,-1));

	int drawAxisType = DrawNone;
	if (plotterSetting.minY * plotterSetting.maxY < 0)
		drawAxisType |= DrawXAxis;
	if (plotterSetting.minX * plotterSetting.maxX < 0)
		drawAxisType |= DrawYAxis;
    if (plotterSetting.minX > 0 || abs(plotterSetting.minX) < EPSION)
        drawAxisType |= DrawLFrameAxis;
    if (plotterSetting.maxX < 0 || abs(plotterSetting.maxX) < EPSION)
        drawAxisType |= DrawRFrameAxis;
    if (plotterSetting.minY > 0 || abs(plotterSetting.minY) < EPSION)
        drawAxisType |= DrawDFrameAxis;
    if (plotterSetting.maxY < 0 || abs(plotterSetting.maxY) < EPSION)
        drawAxisType |= DrawUFrameAxis;



	double oX = rect.left() + abs(plotterSetting.minX) * (rect.width() - 1) / spanx;
	double oY = rect.bottom() - abs(plotterSetting.minY) * (rect.height() - 1) / spany;
	painter->setPen(Qt::black);
	
	if (drawAxisType & DrawXAxis) {
		painter->drawLine(rect.left(), oY, rect.right(), oY);
		for (int i = 0; i <= plotterSetting.numXTicks; i++) {
			int x = rect.left() + (i * (rect.width() - 1) / plotterSetting.numXTicks);
			double label = plotterSetting.minX + i * spanx / plotterSetting.numXTicks;
			painter->drawLine(x, oY, x, oY - 5);
			if (abs(label) > EPSION)
				painter->drawText(x - 50, 
				oY + 5, 
				100, 
				20, 
				Qt::AlignHCenter | 
				Qt::AlignTop, QString::number(label));
		}
	}

	if (drawAxisType & DrawYAxis) {
		painter->drawLine(oX, rect.top(), oX, rect.bottom());
		for (int j = 0; j <= plotterSetting.numYTicks; j++) {
			int y = rect.top() + (j * (rect.height() - 1) / plotterSetting.numYTicks);
			double label = plotterSetting.minY + j * spany / plotterSetting.numYTicks;
			painter->drawLine(oX, y, oX + 5, y);
			if (abs(label) > EPSION)
				painter->drawText(oX - Margin, 
				rect.bottom() + 40 - y, 
				Margin - 5, 20, Qt::AlignVCenter | 
				Qt::AlignRight, QString::number(label));
		}
	}

    if (drawAxisType & DrawFrameAxis) {
        painter->setClipRect(0, 0, width(), height());

        if (drawAxisType & DrawDFrameAxis) {
            painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());		
            for (int i = 0; i <= plotterSetting.numXTicks; i++) {
                int x = rect.left() + (i * (rect.width() - 1) / plotterSetting.numXTicks);
                double label = plotterSetting.minX + i * spanx / plotterSetting.numXTicks;
                painter->drawLine(x, rect.bottom(), x, rect.bottom() - 5);
                if (abs(label) > EPSION)
                    painter->drawText(x - 50, 
                    rect.bottom() + 5, 
                    100, 
                    20, 
                    Qt::AlignHCenter | 
                    Qt::AlignTop, 
                    QString::number(label));
            }
        }

        if (drawAxisType & DrawUFrameAxis) {
            painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            for (int i = 0; i <= plotterSetting.numXTicks; i++) {
                int x = rect.left() + (i * (rect.width() - 1) / plotterSetting.numXTicks);
                double label = plotterSetting.minX + i * spanx / plotterSetting.numXTicks;
                painter->drawLine(x, rect.top(), x, rect.top() + 5);
                if (abs(label) > EPSION)
                    painter->drawText(x - 50, 
                    rect.top() - 15, 
                    100, 
                    20, 
                    Qt::AlignHCenter | 
                    Qt::AlignTop, 
                    QString::number(label)); 
            }
        }

        if (drawAxisType & DrawLFrameAxis) {
            painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            for (int j = 0; j <= plotterSetting.numYTicks; j++) {
                int y = rect.top() + (j * (rect.height() - 1) / plotterSetting.numYTicks);
                double label = plotterSetting.minY + j * spany / plotterSetting.numYTicks;
                painter->drawLine(rect.left(), y, rect.left() + 5, y);
                if (abs(label) > EPSION)
                    painter->drawText(rect.left() - Margin, 
                    rect.bottom() + 40 - y, 
                    Margin - 5, 
                    20, 
                    Qt::AlignVCenter | 
                    Qt::AlignRight, QString::number(label));
            }
        }

        if (drawAxisType & DrawRFrameAxis) {
            painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
            for (int j = 0; j <= plotterSetting.numYTicks; j++) {
                int y = rect.top() + (j * (rect.height() - 1) / plotterSetting.numYTicks);
                double label = plotterSetting.minY + j * spany / plotterSetting.numYTicks;
                painter->drawLine(rect.right(), y, rect.right() - 5, y);
                if (abs(label) > EPSION)
                    painter->drawText(rect.right() - 10, 
                    rect.bottom() + 40 - y, 
                    Margin - 5, 
                    20, 
                    Qt::AlignVCenter | 
                    Qt::AlignRight, QString::number(label));
            }
        }
    }
	if ((plotterSetting.minX * plotterSetting.maxX < 0 || abs(plotterSetting.minX * plotterSetting.maxX) < EPSION) && 
        (plotterSetting.minY * plotterSetting.maxY < 0 || abs(plotterSetting.minY * plotterSetting.maxY) < EPSION)) {
		painter->drawText(oX - Margin, 
			oY - 10, 
			100, 
			20, 
			Qt::AlignVCenter | 
			Qt::AlignHCenter, QString::number(0) );
	}
}

void Plotter::drawCurves(QPainter *painter, const QRect &rect)
{
	static const QColor colorForIds[6] = {
		Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
	};

	calculatePoints(rect);

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
    painter->setPen(QPen(colorForIds[curFormulaType % 6], curveWidth));
    painter->drawPolyline(*polyline);
}

void Plotter::drawRubberBandRect(QPainter *painter, const QRect &rect)
{
    if (rubberBandIsShown && rect.contains(rubberBandRect)) {
        if (rubberBandRect.isValid())
            painter->setPen(Qt::white);
        else
            painter->setPen(Qt::red);
        painter->drawRect(rubberBandRect.normalized().adjusted(0, 0, -1, -1));
    }
}


void Plotter::drawPoint(QPainter *painter, const QRect &rect)
{
    if (pieIsShown && rect.contains(pieO))
    {
        painter->setPen(QPen(Qt::red/*, curveWidth*/));
        painter->setBrush(QBrush(QColor(Qt::red)));
        painter->drawEllipse(pieO, pieRadius, pieRadius);
        bool finded;
        QPointF pointVal = pointValue(pieO.x(), pieO.y(), &finded);
        QString str = QString(tr("Not founded!"));
        if (finded) {
            str = QString(tr("P(%1, %2)")).arg(QString::number(pointVal.x())).arg(QString::number(pointVal.y()));
        }
        //painter->drawRect(pieRect.adjusted(0, 0, -1, -1));
        painter->drawText(pieRect.adjusted(+5, 0, 0, 0), str, Qt::AlignTop | Qt::AlignLeft);
    }
}

QSize Plotter::minimumSizeHint() const
{
	return QSize(10 * Margin, 10 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(10 * Margin, 10 * Margin);
}

void Plotter::setMinX(const QString &strMinX)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    double val = strMinX.toDouble(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.minX = val;
    update();
}

void Plotter::setMaxX(const QString &strMaxX)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    double val = strMaxX.toDouble(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.maxX = val;
    update();
}

void Plotter::setMinY(const QString &strMinY)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    double val = strMinY.toDouble(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.minY = val;
    update();
}

void Plotter::setMaxY(const QString &strMaxY)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    double val = strMaxY.toDouble(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.maxY = val;
    update();
}

void Plotter::setNumXTicks(const QString &strNumXTicks)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    int val = strNumXTicks.toInt(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.numXTicks = val;
    update();
}

void Plotter::setNumYTicks(const QString &strNumYTicks)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    int val = strNumYTicks.toInt(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.numYTicks = val;
    update();
}

void Plotter::setNumXGrid(const QString &strNumXGrid)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    int val = strNumXGrid.toInt(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.numXGrid = val;
    update();
}

void Plotter::setNumYGrid(const QString &strNumYGrid)
{
    bool ok;
    PlotterSetting &plotterSetting = zoomStack[curZoom];
    int val = strNumYGrid.toInt(&ok);
    if (!ok) {
        return ;
    }
    plotterSetting.numYGrid = val;
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

void Plotter::setCurveWidth(const QString &strWidth)
{
    bool ok;
    int val = strWidth.toInt(&ok);
    if (!ok) {
        return ;
    }
    this->curveWidth = val;
    update();
}

void Plotter::setPieRadius(const QString &strRadius)
{
    bool ok;
    int val = strRadius.toInt(&ok);
    if (!ok) {
        return ;
    }
    updatePieRectRegion();
    this->pieRadius = val;
    updatePieRectRegion();
}

void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count() - 1) {
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        update();
    }
}

void Plotter::zoomOut()
{
    if (curZoom > 0) {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        update();
    }
}

void Plotter::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if (event->button() == Qt::LeftButton && rect.contains(event->pos())) {
        rubberBandIsShown = true;
        rubberBandRect.setTopLeft(event->pos());
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
        setCursor(Qt::CrossCursor);
    } else if (event->button() == Qt::RightButton && rect.contains(event->pos())) {
        pieIsShown = true;
        for (auto it = polyline->begin(); it != polyline->end(); it++) {
            if (it->x() >= event->pos().x()) {
                pieO = *it;
                break;
            }
        }
        updatePieRectRegion();
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && rubberBandIsShown) {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();
        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4) {
            return ;
        }

        if (rubberBandRect.isValid()) {
            rect.translate(-Margin, -Margin);
            PlotterSetting prevSetting = zoomStack[curZoom];
            PlotterSetting setting;
            setting.minX = prevSetting.minX + rect.left() * prevSetting.spanX() / (width() - 2 * Margin);
            setting.maxX = prevSetting.minX + rect.right() * prevSetting.spanX() / (width() - 2 * Margin);
            setting.minY = prevSetting.maxY - rect.bottom() * prevSetting.spanY() / (height() - 2 * Margin);
            setting.maxY = prevSetting.maxY - rect.top() * prevSetting.spanY() / (height() - 2 * Margin);
            setting.numXTicks = prevSetting.numXTicks;
            setting.numYTicks = prevSetting.numYTicks;
            setting.numXGrid = prevSetting.numXGrid;
            setting.numYGrid = prevSetting.numYGrid;

            zoomStack.resize(curZoom + 1);
            zoomStack.append(setting);
            zoomIn();
        } else {
            zoomOut();
        }
    } else if (event->button() == Qt::RightButton && pieIsShown) {
        pieIsShown = false;
        updatePieRectRegion();
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }

    if (pieIsShown) {
        updatePieRectRegion();
        for (auto it = polyline->begin(); it != polyline->end(); it++) {
            if (it->x() >= event->pos().x()) {
                pieO = *it;
                break;
            }
        }
        updatePieRectRegion();
    }
}

void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal) {
        zoomStack[curZoom].scroll(numTicks, 0);
    } else {
        zoomStack[curZoom].scroll(0, numTicks);
    }
    update();
}

void Plotter::resizeEvent(QResizeEvent *event)
{
    int x = width() - (zoomInButton->width() + zoomOutButton->width() + 10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
}

void Plotter::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::updatePieRectRegion()
{
    int radius = pieRadius;
    if (radius < 5)
        radius = 5;
    pieRect.setRect(pieO.x(), pieO.y(), 2 * (radius + 1) + 200, 2 * (radius + 1) + 20);
    pieRect.translate(-(radius + 1), -(radius + 1 + 20));
    update(pieRect.normalized());
}


QPointF Plotter::pointValue(const int &x, const int &y, bool *finded/* = 0*/) const
{
    int index = polyline->indexOf(QPoint(x, y));
    if (finded) *finded = false;
    if (index != -1 && curveData.count() > index) {
        if (finded) *finded = true;
        return curveData[index];
    }
    return QPointF(0.0, 0.0);
}

QPoint Plotter::pointPos(const double &valX, const double &valY, bool *finded/* = 0*/) const
{
    int index = curveData.indexOf(QPointF(valX, valY));
    if (finded) *finded = false;
    if (index != -1 && polyline->count() > index) {
        if (finded) *finded = true;
        return (*polyline)[index];
    }
    return QPoint(0, 0);
}

