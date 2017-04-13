#include "myplotter.h"

MyPlotter::MyPlotter(QWidget *parent)
	: QWidget(parent)
{
	createComponents();
	createLayout();
}

void MyPlotter::createComponents()
{
	coordSysLabel = new QLabel(tr("Coordinate Sys: "), this);
	coordSysComboBox = new QComboBox(this);
	QStringList coordSysList;
	coordSysList << "cartesian" << "first quadrant" << "second quadrant" << "thrid quadrant" << "fourth quadrant";
	coordSysComboBox->addItems(coordSysList);

	formulaLabel = new QLabel(tr("Formula: "), this);
	formulaComboBox = new QComboBox(this);
	QStringList formulaList;
	formulaList << "sinx" << "cosx" << "tanx";
	formulaComboBox->addItems(formulaList);

	QRegExpValidator *validator = new QRegExpValidator(QRegExp("\\d"), this);
	xLabel = new QLabel(tr("x-axis width: "), this);
	xLineEdit = new QLineEdit(this);
	xLineEdit->setValidator(validator);

	yLabel = new QLabel(tr("y-axis width: "), this);
	yLineEdit = new QLineEdit(this);
	yLineEdit->setValidator(validator);

	dxLabel = new QLabel(tr("dx: "), this);
	dxLineEdit = new QLineEdit(this);
	dxLineEdit->setValidator(validator);

	dyLabel = new QLabel(tr("dy: "), this);
	dyLineEdit = new QLineEdit(this);
	dyLineEdit->setValidator(validator);

	plotter = new Plotter(this);
}

void MyPlotter::createLayout()
{
	QHBoxLayout *coordSysHLayout = new QHBoxLayout;
	coordSysHLayout->addWidget(coordSysLabel);
	coordSysHLayout->addWidget(coordSysComboBox);
	
	QHBoxLayout *formulaHLayout = new QHBoxLayout;
	formulaHLayout->addWidget(formulaLabel);
	formulaHLayout->addWidget(formulaComboBox);

	QVBoxLayout *coordSysVLayout = new QVBoxLayout;
	coordSysVLayout->addLayout(coordSysHLayout);
	coordSysVLayout->addLayout(formulaHLayout);

	QGroupBox *coordSysGroupBox = new QGroupBox;
	coordSysGroupBox->setTitle(tr("Coord Sys. && Formula"));
	coordSysGroupBox->setLayout(coordSysVLayout);
	
	QHBoxLayout *xHLayout = new QHBoxLayout;
	xHLayout->addWidget(xLabel);
	xHLayout->addWidget(xLineEdit);

	QHBoxLayout *yHLayout = new QHBoxLayout;
	yHLayout->addWidget(yLabel);
	yHLayout->addWidget(yLineEdit);

	QHBoxLayout *dxHLayout = new QHBoxLayout;
	dxHLayout->addWidget(dxLabel);
	dxHLayout->addWidget(dxLineEdit);

	QHBoxLayout *dyHLayout = new QHBoxLayout;
	dyHLayout->addWidget(dyLabel);
	dyHLayout->addWidget(dyLineEdit);

	QVBoxLayout *xyVLayout = new QVBoxLayout;
	xyVLayout->addLayout(xHLayout);
	xyVLayout->addLayout(yHLayout);
	xyVLayout->addLayout(dxHLayout);
	xyVLayout->addLayout(dyHLayout);

	QGroupBox *xyGroupBox = new QGroupBox;
	xyGroupBox->setTitle(tr("X-Y Aisle Setting"));
	xyGroupBox->setLayout(xyVLayout);

	QVBoxLayout *settingLayout = new QVBoxLayout;
	settingLayout->addWidget(coordSysGroupBox);
	settingLayout->addWidget(xyGroupBox);

	QHBoxLayout *plotterLayout = new QHBoxLayout;
	plotterLayout->addWidget(plotter);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(settingLayout);
	mainLayout->addLayout(plotterLayout);

	setLayout(mainLayout);
}

QSize MyPlotter::minimumSizeHint() const
{
	return QSize(7 * Margin, 7 * Margin);
}

QSize MyPlotter::sizeHint() const
{
	return QSize(7 * Margin, 7 * Margin);
}
