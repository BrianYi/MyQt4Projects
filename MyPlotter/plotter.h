#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>

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
    void drawGrid();
    void drawCurves();
    void setCoordSysType(int coordSysType) { curCoordSysType = coordSysType; }
    void setFormulaType(int formulaType) { curFormulaType = formulaType; }
    int coordSysType() const { return curCoordSysType; }
    int formulaType() const { return  curFormulaType; }
protected:
    void paintEvent(QPaintEvent *event); 
private:
    enum { Margin = 50 };
	int curCoordSysType;
    int curFormulaType;
    double maxX;
    double maxY;
    double dX;
    double dY;
    int numXTicks;
    int numYTicks;
};

#endif // PLOTTER_H
