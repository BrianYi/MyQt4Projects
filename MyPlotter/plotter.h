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
	enum DrawAxisType {
		DrawNone = 0x00000000,
		DrawXAxis = 0x00000001, 
		DrawYAxis = 0x00000002, 
		DrawXYAxis = 0x00000003,
		DrawLFrameAxis = 0x00000004,
		DrawRFrameAxis = 0x00000008,
		DrawUFrameAxis = 0x00000010,
		DrawDFrameAxis = 0x00000020
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
	void calculatePoints();
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
	int numXGrid;
	int numYGrid;
	QVector<double> xVec;
	QVector<double> yVec;
	QVector<QPointF> curveData;
};

#endif // PLOTTER_H
