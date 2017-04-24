#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>
#include "node.h"

class Link : public QGraphicsLineItem
{
public:
	Link(Node* from, Node* to);
	~Link();

private:
	Node *fromNode;
	Node *toNode;
};

#endif // LINK_H
