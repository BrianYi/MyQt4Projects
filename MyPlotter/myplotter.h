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
	QSize sizeHint() const;
private:
	enum { Margin = 50 };
	QLabel *coordSysLabel;
	QComboBox *coordSysComboBox;
	QLabel *formulaLabel;
	QComboBox *formulaComboBox;
	QLabel *xLabel;
	QLineEdit *xLineEdit;
	QLabel *yLabel;
	QLineEdit *yLineEdit;
	QLabel *dxLabel;
	QLineEdit *dxLineEdit;
	QLabel *dyLabel;
	QLineEdit *dyLineEdit;
	Plotter *plotter;
};

#endif // MYPLOTTER_H
