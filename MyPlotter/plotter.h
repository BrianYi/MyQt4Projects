#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QtGui>

#define EPSION 0.0001

class Plotter : public QWidget
{
	Q_OBJECT
public:
    enum CoordianteSysType { 
        Cartesian, FirstQuadrant, SecondQuadrant, thridQuadrant, fourthQuadrant 
    };
    enum FormulaType {
        SinX, CosX, TanX
    };
public:
	Plotter(QWidget *parent = 0);
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
    void setCoordSysType(int coordSysType) { curCoordSysType = coordSysType; }
    void setFormulaType(int formulaType) { curFormulaType = formulaType; }
    int coordSysType() const { return curCoordSysType; }
    int formulaType() const { return  curFormulaType; }
	double spanX() const { return maxX - minX; }
	double spanY() const { return maxY - minY; }
	QSize minimumSizeHint() const;
protected:
    void paintEvent(QPaintEvent *event); 
private:
    enum { Margin = 50 };
	int curCoordSysType;
    int curFormulaType;
	double minX;
	double minY;
    double maxX;
    double maxY;
    int numXTicks;
    int numYTicks;
};

#endif // PLOTTER_H
