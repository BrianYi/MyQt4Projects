#include "plotter.h"

Plotter::Plotter(QWidget *parent)
	: QWidget(parent)
{
    curCoordSysType = Cartesian;
    curFormulaType = SinX;
    maxX = 10.0;
    maxY = 10.0;
    double dX;
    double dY;
    int numXTicks;
    int numYTicks;
}

void Plotter::paintEvent(QPaintEvent *event)
{

}

void Plotter::drawGrid()
{

}

void Plotter::drawCurves()
{

}

