#ifndef MYPLOTTER_H
#define MYPLOTTER_H

#include <QWidget>
#include <QtGui>
#include "plotter.h"
class MyPlotter : public QWidget
{
	Q_OBJECT

public:
	enum CoordianteSysType { 
		Cartesian, FirstQuadrant, SecondQuadrant, thridQuadrant, fourthQuadrant 
	};
	enum FormulaType {
		SinX, CosX, TanX
	};
	MyPlotter(QWidget *parent = 0);
	void createComponents();
	void createLayout();
    void createConnections();
    void createInitialValues();
	int curFormulaType() const { return formulaComboBox->currentIndex(); }
	QSize minimumSizeHint() const;
    QSize sizeHint() const;
signals:
    void setMinX(const QString &);
    void setMaxX(const QString &);
    void setMinY(const QString &);
    void setMaxY(const QString &);
    void setNumXTicks(const QString &);
    void setNumYTicks(const QString &);
    void setNumXGrid(const QString &);
    void setNumYGrid(const QString &);
    void setCoordSysType(int coordSysType);
    void setFormulaType(int formulaType);
    void setCurveSmoothLevelType(int curveSmoothLvlType);
    void setCurveWidth(const QString &);
    void setPieRadius(const QString &);
protected slots:
    void minXChanged(const QString &strMinX);
    void maxXChanged(const QString &strMaxX);
    void minYChanged(const QString &strMinY);
    void maxYChanged(const QString &strMaxY);
    void numXTicksChanged(const QString &strNumXTicks);
    void numYTicksChanged(const QString &strNumYTicks);
    void numXGridChanged(const QString &strNumXGrid);
    void numYGridChanged(const QString &strNumYGrid);
    void coordSysTypeChanged(int coordSysType);
    void formulaTypeChanged(int formulaType);
    void curveSmoothLevelTypeChanged(int index);
    void curveWidthChanged(const QString &strWidth);
    void pieRadiusChanged(const QString &strRadius);
private:
	QRegExpValidator *realValidator;
	QRegExpValidator *nonegRealValidator;
	QRegExpValidator *nonegDigitalValidator;
	QLabel *coordSysLabel;
	QComboBox *coordSysComboBox;
	QLabel *formulaLabel;
	QComboBox *formulaComboBox;
	QLabel *xLabel;
	QLineEdit *xMinLineEdit;
	QLabel *xlb;
	QLineEdit *xMaxLineEdit;
	QLabel *yLabel;
	QLineEdit *yMinLineEdit;
	QLabel *ylb;
	QLineEdit *yMaxLineEdit;
	QLabel *numXTicksLabel;
	QLineEdit *numXTicksLineEdit;
	QLabel *numYTicksLabel;
	QLineEdit *numYTicksLineEdit;
	QLabel *numGridLabel;
	QLabel *gridLabel;
	QLineEdit *numXGridLineEdit;
	QLineEdit *numYGridLineEdit;
    QLabel *curveSmoothLvlLabel;
    QComboBox *curveSmoothLvlComboBox;
    QLabel *curveWidthLabel;
    QLineEdit *curveWidthLineEdit;
    QLabel *pieRadiusLabel;
    QLineEdit *pieRadiusLineEdit;
	Plotter *plotter;
};

#endif // MYPLOTTER_H
