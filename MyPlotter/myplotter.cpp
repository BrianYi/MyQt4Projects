#include "myplotter.h"

static const Plotter::CurveSmoothLevelType sCurveSmoothLevelType[Plotter::SmoothLevelNum] = {
    Plotter::SmoothLevel0,
    Plotter::SmoothLevel1,
    Plotter::SmoothLevel2,
    Plotter::SmoothLevel3,
    Plotter::SmoothLevel4,
    Plotter::SmoothLevel5,
    Plotter::SmoothLevel6,
    Plotter::SmoothLevel7,
    Plotter::SmoothLevel8,
    Plotter::SmoothLevel9
};

MyPlotter::MyPlotter(QWidget *parent)
	: QWidget(parent)
{
	createComponents();
	createLayout();
    createConnections();
    createInitialValues();
}

void MyPlotter::createComponents()
{
	coordSysLabel = new QLabel(tr("coordinate system:"), this);
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

	realValidator = new QRegExpValidator(QRegExp("^(-{0,1}\\d{1,3})(.\\d{2})\\?$"), this);
	nonegRealValidator = new QRegExpValidator(QRegExp("^(\\d{1,3})(.\\d{2})\\?$"), this);
	nonegDigitalValidator = new QRegExpValidator(QRegExp("[1-9]\\d{1,2}"), this);
	xLabel = new QLabel(tr("x-axis range from:"), this);
    xLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //xLabel->setFixedSize(xLabel->sizeHint());
	xMinLineEdit = new QLineEdit(this);
    xMinLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    xMinLineEdit->setFixedWidth(50);
	xMinLineEdit->setValidator(realValidator);
    //xMinLineEdit->setText("-10.00");
    //xLineEdit->setFixedWidth(Margin);
	
	xlb = new QLabel(tr("to"));
	xlb->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	xMaxLineEdit = new QLineEdit(this);
	xMaxLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	xMaxLineEdit->setFixedWidth(50);
	xMaxLineEdit->setValidator(realValidator);
    //xMaxLineEdit->setText("10.00");

	yLabel = new QLabel(tr("y-axis range from:"), this);
    yLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //yLabel->setFixedSize(yLabel->sizeHint());
	yMinLineEdit = new QLineEdit(this);
    yMinLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    yMinLineEdit->setFixedWidth(50);
	yMinLineEdit->setValidator(realValidator);
    //yLineEdit->setFixedWidth(Margin);
    //yMinLineEdit->setText("-10.00");

	ylb = new QLabel(tr("to"));
	ylb->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	yMaxLineEdit = new QLineEdit(this);
	yMaxLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	yMaxLineEdit->setFixedWidth(50);
	yMaxLineEdit->setValidator(realValidator);
    //yMaxLineEdit->setText("10.00");

	numXTicksLabel = new QLabel(tr("num ticks(x-axis):"), this);
    numXTicksLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //numXTicksLabel->setFixedSize(numXTicksLabel->sizeHint());
	numXTicksLineEdit = new QLineEdit(this);
    numXTicksLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    numXTicksLineEdit->setFixedWidth(50);
	numXTicksLineEdit->setValidator(nonegDigitalValidator);
    //numXTicksLineEdit->setFixedWidth(Margin);
    //numXTicksLineEdit->setText("20");

	numYTicksLabel = new QLabel(tr("num ticks(y-axis):"), this);
    numYTicksLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //numYTicksLabel->setFixedSize(numYTicksLabel->sizeHint());
	numYTicksLineEdit = new QLineEdit(this);
    numYTicksLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    numYTicksLineEdit->setFixedWidth(50);
	numYTicksLineEdit->setValidator(nonegDigitalValidator);
    //numYTicksLineEdit->setFixedWidth(Margin);
    //numYTicksLineEdit->setText("20");

	numGridLabel = new QLabel(tr("grid size is:"), this);
	numGridLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	
	gridLabel = new QLabel(tr("x"), this);
	gridLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	numXGridLineEdit = new QLineEdit(this);
	numXGridLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	numXGridLineEdit->setFixedWidth(30);
	numXGridLineEdit->setValidator(nonegDigitalValidator);
    //numXGridLineEdit->setText("40");

	numYGridLineEdit = new QLineEdit(this);
	numYGridLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	numYGridLineEdit->setFixedWidth(30);
	numYGridLineEdit->setValidator(nonegDigitalValidator);
    //numYGridLineEdit->setText("40");

    curveSmoothLvlLabel = new QLabel(tr("curve smooth level:"), this);
    curveSmoothLvlLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    curveSmoothLvlComboBox = new QComboBox(this);
    QStringList curveSmoothLvlList;
    for (int i = 0; i < Plotter::SmoothLevelNum; i++) {
        curveSmoothLvlList << QString("SmoothLevel") + QString::number(i);
    }
    curveSmoothLvlComboBox->addItems(curveSmoothLvlList);
    curveSmoothLvlComboBox->setFixedWidth(150);

    curveWidthLabel = new QLabel(tr("curve with(pixels):"), this);
    curveWidthLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    curveWidthLineEdit = new QLineEdit(this);
    curveWidthLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    curveWidthLineEdit->setFixedWidth(50);
    curveWidthLineEdit->setValidator(nonegDigitalValidator);

    pieRadiusLabel = new QLabel(tr("pie radius(pixels):"), this);
    pieRadiusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    pieRadiusLineEdit = new QLineEdit(this);
    pieRadiusLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    pieRadiusLineEdit->setFixedWidth(50);
    pieRadiusLineEdit->setValidator(nonegDigitalValidator);

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

	QGroupBox *coordSysGroupBox = new QGroupBox(this);
	coordSysGroupBox->setTitle(tr("Coord Sys. && Formula"));
	coordSysGroupBox->setLayout(coordSysVLayout);
    coordSysGroupBox->setFixedWidth(coordSysGroupBox->sizeHint().width());
	
	QHBoxLayout *xHLayout = new QHBoxLayout;
	xHLayout->addWidget(xLabel);
	xHLayout->addWidget(xMinLineEdit);
	xHLayout->addWidget(xlb);
	xHLayout->addWidget(xMaxLineEdit);

	QHBoxLayout *yHLayout = new QHBoxLayout;
	yHLayout->addWidget(yLabel);
	yHLayout->addWidget(yMinLineEdit);
	yHLayout->addWidget(ylb);
	yHLayout->addWidget(yMaxLineEdit);

	QHBoxLayout *numXTicksHLayout = new QHBoxLayout;
	numXTicksHLayout->addWidget(numXTicksLabel);
	numXTicksHLayout->addWidget(numXTicksLineEdit);

	QHBoxLayout *numYTicksHLayout = new QHBoxLayout;
	numYTicksHLayout->addWidget(numYTicksLabel);
	numYTicksHLayout->addWidget(numYTicksLineEdit);

// 	QHBoxLayout *numGridHLayout = new QHBoxLayout;
// 	numGridHLayout->addWidget(numGridLabel);
// 	numGridHLayout->addWidget(numXGridLineEdit);
// 	numGridHLayout->addWidget(gridLabel);
// 	numGridHLayout->addWidget(numYGridLineEdit);

	QVBoxLayout *xyVLayout = new QVBoxLayout;
	xyVLayout->addLayout(xHLayout);
	xyVLayout->addLayout(yHLayout);
	xyVLayout->addLayout(numXTicksHLayout);
	xyVLayout->addLayout(numYTicksHLayout);
//	xyVLayout->addLayout(numGridHLayout);

	QGroupBox *xyGroupBox = new QGroupBox(this);
	xyGroupBox->setTitle(tr("X-Y Aisle Setting"));
	xyGroupBox->setLayout(xyVLayout);
    xyGroupBox->setFixedWidth(xyGroupBox->sizeHint().width());

    QHBoxLayout *numGridHLayout = new QHBoxLayout;
    numGridHLayout->addWidget(numGridLabel);
    numGridHLayout->addWidget(numXGridLineEdit);
    numGridHLayout->addWidget(gridLabel);
    numGridHLayout->addWidget(numYGridLineEdit);

    QHBoxLayout *curveSmoothLvlHLayout = new QHBoxLayout;
    curveSmoothLvlHLayout->addWidget(curveSmoothLvlLabel);
    curveSmoothLvlHLayout->addWidget(curveSmoothLvlComboBox);

    QHBoxLayout *curveWidthHLayout = new QHBoxLayout;
    curveWidthHLayout->addWidget(curveWidthLabel);
    curveWidthHLayout->addWidget(curveWidthLineEdit);

    QHBoxLayout *pieRadiusHLayout = new QHBoxLayout;
    pieRadiusHLayout->addWidget(pieRadiusLabel);
    pieRadiusHLayout->addWidget(pieRadiusLineEdit);

    QVBoxLayout *othersVLayout = new QVBoxLayout;
    othersVLayout->addLayout(numGridHLayout);
    othersVLayout->addLayout(curveSmoothLvlHLayout);
    othersVLayout->addLayout(curveWidthHLayout);
    othersVLayout->addLayout(pieRadiusHLayout);

    QGroupBox *othersGroupBox = new QGroupBox(this);
    othersGroupBox->setTitle(tr("Others"));
    othersGroupBox->setLayout(othersVLayout);
    othersGroupBox->setFixedWidth(othersGroupBox->sizeHint().width());

	QVBoxLayout *settingLayout = new QVBoxLayout;
	settingLayout->addWidget(coordSysGroupBox);
	settingLayout->addWidget(xyGroupBox);
    settingLayout->addWidget(othersGroupBox);
    settingLayout->setStretch(0, 1);
    settingLayout->setStretch(1, 2);
    settingLayout->setStretch(2, 1);

	QHBoxLayout *plotterLayout = new QHBoxLayout;
	plotterLayout->addWidget(plotter);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(settingLayout);
	mainLayout->addLayout(plotterLayout);
    
	setLayout(mainLayout);
}

void MyPlotter::createConnections()
{
    connect(coordSysComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(coordSysTypeChanged(int)));
    connect(formulaComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(formulaTypeChanged(int)));
    connect(this, SIGNAL(setCoordSysType(int)), plotter, SLOT(setCoordSysType(int)));
    connect(this, SIGNAL(setFormulaType(int)), plotter, SLOT(setFormulaType(int)));
    connect(xMinLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(minXChanged(const QString &)));
    connect(xMaxLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(maxXChanged(const QString &)));
    connect(yMinLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(minYChanged(const QString &)));
    connect(yMaxLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(maxYChanged(const QString &)));
    connect(this, SIGNAL(setMinX(const QString &)), plotter, SLOT(setMinX(const QString &)));
    connect(this, SIGNAL(setMaxX(const QString &)), plotter, SLOT(setMaxX(const QString &)));
    connect(this, SIGNAL(setMinY(const QString &)), plotter, SLOT(setMinY(const QString &)));
    connect(this, SIGNAL(setMaxY(const QString &)), plotter, SLOT(setMaxY(const QString &)));
    connect(this, SIGNAL(setNumXTicks(const QString &)), plotter, SLOT(setNumXTicks(const QString &)));
    connect(this, SIGNAL(setNumYTicks(const QString &)), plotter, SLOT(setNumYTicks(const QString &)));
    connect(this, SIGNAL(setNumXGrid(const QString &)), plotter, SLOT(setNumXGrid(const QString &)));
    connect(this, SIGNAL(setNumYGrid(const QString &)), plotter, SLOT(setNumYGrid(const QString &)));
    connect(numXTicksLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(numXTicksChanged(const QString &)));
    connect(numYTicksLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(numYTicksChanged(const QString &)));
    connect(numXGridLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(numXGridChanged(const QString &)));
    connect(numYGridLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(numYGridChanged(const QString &)));
    connect(curveSmoothLvlComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(curveSmoothLevelTypeChanged(int)));
    connect(this, SIGNAL(setCurveSmoothLevelType(int)), plotter, SLOT(setCurveSmoothLevelType(int)));
    connect(curveWidthLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(curveWidthChanged(const QString &)));
    connect(pieRadiusLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(pieRadiusChanged(const QString &)));
    connect(this, SIGNAL(setCurveWidth(const QString &)), plotter, SLOT(setCurveWidth(const QString &)));
    connect(this, SIGNAL(setPieRadius(const QString &)), plotter, SLOT(setPieRadius(const QString &)));
}

void MyPlotter::createInitialValues()
{
    xMinLineEdit->setText("-10.00");
    xMaxLineEdit->setText("10.00");
    yMinLineEdit->setText("-10.00");
    yMaxLineEdit->setText("10.00");
    numXTicksLineEdit->setText("20");
    numYTicksLineEdit->setText("20");
    numXGridLineEdit->setText("40");
    numYGridLineEdit->setText("40");
    curveWidthLineEdit->setText("1");
    pieRadiusLineEdit->setText("5");
}

QSize MyPlotter::minimumSizeHint() const
{
	return QSize(500, 500);
}

QSize MyPlotter::sizeHint() const
{
    return QSize(500, 500);
}


void MyPlotter::minXChanged(const QString &strMinX)
{
    if (strMinX.isEmpty() || strMinX == "-")
        return ;
    QString strMaxX = xMaxLineEdit->text();
    if (!strMaxX.isEmpty()) {
        double maxX = strMaxX.toDouble();
        double minX = strMinX.toDouble();
        if (minX > maxX || abs(minX - maxX) < EPSION)
            xMinLineEdit->clear();
    }
    emit setMinX(strMinX);
}

void MyPlotter::maxXChanged(const QString &strMaxX)
{
    if (strMaxX.isEmpty() || strMaxX == "-")
        return ;
    QString strMinX = xMinLineEdit->text();
    if (!strMinX.isEmpty()) {
        double maxX = strMaxX.toDouble();
        double minX = strMinX.toDouble();
        if (minX > maxX || abs(minX - maxX) < EPSION)
            xMaxLineEdit->clear();
    }
    emit setMaxX(strMaxX);
}

void MyPlotter::minYChanged(const QString &strMinY)
{
    if (strMinY.isEmpty() || strMinY == "-")
        return ;
    QString strMaxY = yMaxLineEdit->text();
    if (!strMaxY.isEmpty()) {
        double maxY = strMaxY.toDouble();
        double minY= strMinY.toDouble();
        if (minY > maxY || abs(minY - maxY) < EPSION)
            yMinLineEdit->clear();
    }
    emit setMinY(strMinY);
}

void MyPlotter::maxYChanged(const QString &strMaxY)
{
    if (strMaxY.isEmpty() || strMaxY == "-")
        return ;
    QString strMinY = yMinLineEdit->text();
    if (!strMinY.isEmpty()) {
        double maxY = strMaxY.toDouble();
        double minY = strMinY.toDouble();
        if (minY > maxY || abs(minY - maxY) < EPSION)
            yMaxLineEdit->clear();
    }
    emit setMaxY(strMaxY);
}

void MyPlotter::numXTicksChanged(const QString &strNumXTicks)
{
    if (strNumXTicks.isEmpty())
        return ;
    emit setNumXTicks(strNumXTicks);
}

void MyPlotter::numYTicksChanged(const QString &strNumYTicks)
{
    if (strNumYTicks.isEmpty()) 
        return ;
    emit setNumYTicks(strNumYTicks);
}

void MyPlotter::numXGridChanged(const QString &strNumXGrid)
{
    if (strNumXGrid.isEmpty())
        return ;
    emit setNumXGrid(strNumXGrid);
}

void MyPlotter::numYGridChanged(const QString &strNumYGrid)
{
    if (strNumYGrid.isEmpty())
        return ;
    emit setNumYGrid(strNumYGrid);
}

void MyPlotter::coordSysTypeChanged(int coordSysType)
{
    switch (coordSysType)
    {
    case Cartesian:
        {
            QString strXMin = xMinLineEdit->text();
            QString strXMax = xMaxLineEdit->text();
            QString strYMin = yMinLineEdit->text();
            QString strYMax = yMaxLineEdit->text();
            double minX = strXMin.toDouble();
            double maxX = strXMax.toDouble();
            double minY = strYMin.toDouble();
            double maxY = strYMax.toDouble();
            if (strXMin.isEmpty() || minX > 0 || abs(minX) < EPSION) {
                xMinLineEdit->setText("-10.00");
            }
            if (strXMax.isEmpty() || maxX < 0 || abs(maxX) < EPSION) {
                xMaxLineEdit->setText("10.00");
            }
            if (strYMin.isEmpty() || minY > 0 || abs(minY) < EPSION) {
                yMinLineEdit->setText("-10.00");
            }
            if (strYMax.isEmpty() || maxY < 0 || abs(maxY) < EPSION) {
                yMaxLineEdit->setText("10.00");
            }
            xMinLineEdit->setEnabled(true);
            xMaxLineEdit->setEnabled(true);
            yMinLineEdit->setEnabled(true);
            yMaxLineEdit->setEnabled(true);
            break;
        }
    case FirstQuadrant:
        {
            QString strXMax = xMaxLineEdit->text();
            QString strYMax = yMaxLineEdit->text();
            double maxX = strXMax.toDouble();
            double maxY = strYMax.toDouble();
            xMinLineEdit->setText("");
            yMinLineEdit->setText("");
            if (strXMax.isEmpty() || maxX < 0 || abs(maxX) < EPSION) {
                xMaxLineEdit->setText("10.00");
            }
            if (strYMax.isEmpty() || maxY < 0 || abs(maxY) < EPSION) {
                yMaxLineEdit->setText("10.00");
            }
            xMinLineEdit->setText("0.00");
            yMinLineEdit->setText("0.00");
            xMinLineEdit->setEnabled(false);
            xMaxLineEdit->setEnabled(true);
            yMinLineEdit->setEnabled(false);
            yMaxLineEdit->setEnabled(true);
            break;
        }
    case SecondQuadrant:
        {
            QString strXMin = xMaxLineEdit->text();
            QString strYMax = yMaxLineEdit->text();
            double minX = strXMin.toDouble();
            double maxY = strYMax.toDouble();
            xMaxLineEdit->setText("");
            yMinLineEdit->setText("");
            if (strXMin.isEmpty() || minX > 0 || abs(minX) < EPSION) {
                xMinLineEdit->setText("-10.00");
            }
            if (strYMax.isEmpty() || maxY < 0 || abs(maxY) < EPSION) {
                yMaxLineEdit->setText("10.00");
            }
            xMaxLineEdit->setText("0.00");
            yMinLineEdit->setText("0.00");
            xMinLineEdit->setEnabled(true);
            xMaxLineEdit->setEnabled(false);
            yMinLineEdit->setEnabled(false);
            yMaxLineEdit->setEnabled(true);
            break;
        }
    case thridQuadrant:
        {
            QString strXMin = xMinLineEdit->text();
            QString strYMin = yMinLineEdit->text();
            double minX = strXMin.toDouble();
            double minY = strYMin.toDouble();
            xMaxLineEdit->setText("");
            yMaxLineEdit->setText("");
            if (strXMin.isEmpty() || minX > 0 || abs(minX) < EPSION) {
                xMinLineEdit->setText("-10.00");
            }
            if (strYMin.isEmpty() || minY > 0 || abs(minY) < EPSION) {
                yMinLineEdit->setText("-10.00");
            }
            xMaxLineEdit->setText("0.00");
            yMaxLineEdit->setText("0.00");
            xMinLineEdit->setEnabled(true);
            xMaxLineEdit->setEnabled(false);
            yMinLineEdit->setEnabled(true);
            yMaxLineEdit->setEnabled(false);
            break;
        }
    case fourthQuadrant:
        {
            QString strXMax = xMaxLineEdit->text();
            QString strYMin = yMinLineEdit->text();
            double maxX = strXMax.toDouble();
            double minY = strYMin.toDouble();
            xMinLineEdit->setText("");
            yMaxLineEdit->setText("");
            if (strXMax.isEmpty() || maxX < 0 || abs(maxX) < EPSION) {
                xMaxLineEdit->setText("10.00");
            }
            if (strYMin.isEmpty() || minY > 0 || abs(minY) < EPSION) {
                yMinLineEdit->setText("-10.00");
            }
            xMinLineEdit->setText("0.00");
            yMaxLineEdit->setText("0.00");
            xMinLineEdit->setEnabled(false);
            xMaxLineEdit->setEnabled(true);
            yMinLineEdit->setEnabled(true);
            yMaxLineEdit->setEnabled(false);
            break;
        }
    default:
        break;
    }
    emit setCoordSysType(coordSysType);
}

void MyPlotter::formulaTypeChanged(int formulaType)
{
    emit setFormulaType(formulaType);
}


void MyPlotter::curveSmoothLevelTypeChanged(int index)
{
    emit setCurveSmoothLevelType(sCurveSmoothLevelType[index]);		
}

void MyPlotter::curveWidthChanged(const QString &strWidth)
{
    if (strWidth.isEmpty())
        return ;
    emit setCurveWidth(strWidth);
}

void MyPlotter::pieRadiusChanged(const QString &strRadius)
{
    if (strRadius.isEmpty())
        return ;
    emit setPieRadius(strRadius);
}


