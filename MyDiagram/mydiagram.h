#ifndef MYDIAGRAM_H
#define MYDIAGRAM_H

#include <QMainWindow>
#include "node.h"

class MyDiagram : public QMainWindow
{
	Q_OBJECT

public:
	MyDiagram(QWidget *parent = 0);
	~MyDiagram();

	void setupNode(Node *node);

	void createActions();
	void createMenus();
	void createToolbars();
	void createStatusbar();
	public slots:
		Node *addNode();
private:
	QGraphicsView *view;
	QGraphicsScene *scene;
	QList<Node *> nodes;
	qint32 seqNumber;
	QAction *addNodeAction;
};

#endif // MYDIAGRAM_H
