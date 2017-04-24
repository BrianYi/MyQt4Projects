#include "node.h"

Node::Node()
{
	myOutlineColor = Qt::red;
	myBackgroundColor = Qt::white;
	myTextColor = Qt::darkBlue;
	
	setFlags(ItemIsMovable | ItemIsSelectable);
}

Node::~Node()
{

}

void Node::setText(const QString &text)
{
	myText = text;
}

QString Node::text() const
{
	return myText;
}

void Node::setOutlineColor(const QColor &outlineColor)
{
	myOutlineColor = outlineColor;
}

QColor Node::outlineColor() const
{
	return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &backgroundColor)
{
	myBackgroundColor = backgroundColor;
}

QColor Node::backgroundColor() const
{
	return myBackgroundColor;
}

void Node::setTextColor(const QColor &textColor)
{
	myTextColor = textColor;
}

QColor Node::textColor() const
{
	return myTextColor;
}

QList<Link*> Node::links()
{
	return myLinks;
}

void Node::addLink(Link* link)
{
	myLinks.append(link);
}

bool Node::removeLink(Link* link)
{
	return myLinks.removeOne(link);
}

QRectF Node::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

// QPainterPath Node::shape() const
// {
// 
// }

QRectF Node::outlineRect() const
{
	const int Padding = 8;
	QFontMetricsF metrics = qApp->font();
	QRectF rect = metrics.boundingRect(myText);
	rect.adjust(-Padding, -Padding, Padding, Padding);
	rect.translate(-rect.center());
	return rect;
}

void Node::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen(myOutlineColor);
	if (option->state & QStyle::State_Selected) {
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
	}

	painter->setPen(pen);
	painter->setBrush(myBackgroundColor);

	QRectF rect = outlineRect();
	painter->drawRoundedRect(rect, 10, 10);

	painter->setPen(myTextColor);
	painter->drawText(rect, Qt::AlignCenter, myText);
}