#include "myplotter.h"

MyPlotter::MyPlotter(QWidget *parent)
	: QWidget(parent)
{
	createComponents();
	createLayout();
}

void MyPlotter::createComponents()
{
	coordSysLabel = new QLabel(tr("coordinate system: "), this);
    coordSysLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //coordSysLabel->setFixedSize(coordSysLabel->sizeHint());
	coordSysComboBox = new QComboBox(this);
	QStringList coordSysList;
	coordSysList << "cartesian" << "first quadrant" << "second quadrant" << "thrid quadrant" << "fourth quadrant";
	coordSysComboBox->addItems(coordSysList);
    coordSysComboBox->setFixedWidth(150);

	formulaLabel = new QLabel(tr("formula: "), this);
    formulaLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //formulaLabel->setFixedSize(formulaLabel->sizeHint());
	formulaComboBox = new QComboBox(this);
	QStringList formulaList;
	formulaList << "sinx" << "cosx" << "tanx";
	formulaComboBox->addItems(formulaList);
    formulaComboBox->setFixedWidth(150);
    //formulaComboBox->setFixedSize(formulaComboBox->sizeHint());

    QRegExpValidator *validator = new QRegExpValidator(QRegExp("(\\d{1,3})(.\\d{2})\\?"), this);
	xLabel = new QLabel(tr("x-axis width: "), this);
    xLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //xLabel->setFixedSize(xLabel->sizeHint());
	xLineEdit = new QLineEdit(this);
    xLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    xLineEdit->setFixedWidth(50);
	xLineEdit->setValidator(validator);
    //xLineEdit->setFixedWidth(Margin);

	yLabel = new QLabel(tr("y-axis width: "), this);
    yLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //yLabel->setFixedSize(yLabel->sizeHint());
	yLineEdit = new QLineEdit(this);
    yLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    yLineEdit->setFixedWidth(50);
	yLineEdit->setValidator(validator);
    //yLineEdit->setFixedWidth(Margin);

	numXTicksLabel = new QLabel(tr("num ticks(x-axis): "), this);
    numXTicksLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //numXTicksLabel->setFixedSize(numXTicksLabel->sizeHint());
	numXTicksLineEdit = new QLineEdit(this);
    numXTicksLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    numXTicksLineEdit->setFixedWidth(50);
	numXTicksLineEdit->setValidator(validator);
    //numXTicksLineEdit->setFixedWidth(Margin);

	numYTicksLabel = new QLabel(tr("num ticks(y-axis): "), this);
    numYTicksLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //numYTicksLabel->setFixedSize(numYTicksLabel->sizeHint());
	numYTicksLineEdit = new QLineEdit(this);
    numYTicksLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    numYTicksLineEdit->setFixedWidth(50);
	numYTicksLineEdit->setValidator(validator);
    //numYTicksLineEdit->setFixedWidth(Margin);

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
    //coordSysGroupBox->setFixedSize(coordSysGroupBox->sizeHint());
	
	QHBoxLayout *xHLayout = new QHBoxLayout;
	xHLayout->addWidget(xLabel);
	xHLayout->addWidget(xLineEdit);

	QHBoxLayout *yHLayout = new QHBoxLayout;
	yHLayout->addWidget(yLabel);
	yHLayout->addWidget(yLineEdit);

	QHBoxLayout *dxHLayout = new QHBoxLayout;
	dxHLayout->addWidget(numXTicksLabel);
	dxHLayout->addWidget(numXTicksLineEdit);

	QHBoxLayout *dyHLayout = new QHBoxLayout;
	dyHLayout->addWidget(numYTicksLabel);
	dyHLayout->addWidget(numYTicksLineEdit);

	QVBoxLayout *xyVLayout = new QVBoxLayout;
	xyVLayout->addLayout(xHLayout);
	xyVLayout->addLayout(yHLayout);
	xyVLayout->addLayout(dxHLayout);
	xyVLayout->addLayout(dyHLayout);

	QGroupBox *xyGroupBox = new QGroupBox;
	xyGroupBox->setTitle(tr("X-Y Aisle Setting"));
	xyGroupBox->setLayout(xyVLayout);
    //xyGroupBox->setFixedSize(xyGroupBox->sizeHint());

	QVBoxLayout *settingLayout = new QVBoxLayout;
	settingLayout->addWidget(coordSysGroupBox);
	settingLayout->addWidget(xyGroupBox);
    settingLayout->setStretch(0, 1);
    settingLayout->setStretch(1, 2);

	QHBoxLayout *plotterLayout = new QHBoxLayout;
	plotterLayout->addWidget(plotter);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(settingLayout);
	mainLayout->addLayout(plotterLayout);
    
	setLayout(mainLayout);
}

QSize MyPlotter::minimumSizeHint() const
{
	return sizeHint();
}

