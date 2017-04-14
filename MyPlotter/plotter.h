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
		DrawDFrameAxis = 0x00000020,
        DrawFrameAxis = DrawLFrameAxis | DrawRFrameAxis | DrawUFrameAxis | DrawDFrameAxis
	};
    enum CurveSmoothLevelType {
        SmoothLevel0 = 10,
        SmoothLevel1 = 50,
        SmoothLevel2 = 100,
        SmoothLevel3 = 300,
        SmoothLevel4 = 500,
        SmoothLevel5 = 800,
        SmoothLevel6 = 1000,
        SmoothLevel7 = 3000,
        SmoothLevel8 = 5000,
        SmoothLevel9 = 10000,
        SmoothLevelNum = 10
    };
public:
	Plotter(QWidget *parent = 0);
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
    int coordSysType() const { return curCoordSysType; }
    int formulaType() const { return  curFormulaType; }
	double spanX() const { return maxX - minX; }
	double spanY() const { return maxY - minY; }
	QSize minimumSizeHint() const;
public slots:
    void setMinX(const QString &strMinX);
    void setMaxX(const QString &strMaxX);
    void setMinY(const QString &strMinY);
    void setMaxY(const QString &strMaxY);
    void setNumXTicks(const QString &strNumXTicks);
    void setNumYTicks(const QString &strNumYTicks);
    void setNumXGrid(const QString &strNumXGrid);
    void setNumYGrid(const QString &strNumYGrid);
    void setCoordSysType(int coordSysType);
    void setFormulaType(int formulaType);
    void setCurveSmoothLevelType(int curveSmoothLvlType);
protected:
    void paintEvent(QPaintEvent *event); 
	void calculatePoints();
    double calculate(const double &val);
private:
    enum { Margin = 50 };
	int curCoordSysType;
    int curFormulaType;
    int curCurveSmoothLevelType;
	double minX;
	double minY;
    double maxX;
    double maxY;
    int numXTicks;
    int numYTicks;
	int numXGrid;
    int numYGrid;
	QPolygonF *polyline;
};

#endif // PLOTTER_H
