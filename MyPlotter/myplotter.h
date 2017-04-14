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
	int curFormulaType() const { return formulaComboBox->currentIndex(); }
	QSize minimumSizeHint() const;
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
	Plotter *plotter;
};

#endif // MYPLOTTER_H
