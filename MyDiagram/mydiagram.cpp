#include "mydiagram.h"

MyDiagram::MyDiagram(QWidget *parent)
	: QMainWindow(parent)
{
	scene = new QGraphicsScene(0, 0, 500, 500, this);

	view = new QGraphicsView(scene, this);
	view->setDragMode(QGraphicsView::RubberBandDrag);
	view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	setCentralWidget(view);

	seqNumber = 0;

	createActions();
	createMenus();
	createToolbars();
	createStatusbar();

	connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
}

MyDiagram::~MyDiagram()
{
	qDeleteAll(nodes);
}

void MyDiagram::setupNode(Node *node)
{
	QString nodeName = tr("Node%1").arg(QString::number(seqNumber));
	node->setText(nodeName);
	node->setPos(30 + 100 * (seqNumber % 5), 30 + 50 * (seqNumber / 5));
	nodes.append(node);
	scene->addItem(node);
	seqNumber++;
}

void MyDiagram::createActions()
{
	addNodeAction = new QAction(tr("Add node"), this);
}

void MyDiagram::createMenus()
{

}

void MyDiagram::createToolbars()
{
	QToolBar *myToolBar = addToolBar(tr("My toolbar"));
	myToolBar->addAction(addNodeAction);
}

void MyDiagram::createStatusbar()
{

}

Node * MyDiagram::addNode()
{
	Node *node = new Node;
	setupNode(node);
	return node;
}
