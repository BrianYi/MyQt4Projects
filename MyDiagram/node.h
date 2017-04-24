#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QtGui>
#include <QFontMetricsF>
class Link;

class Node : public QGraphicsItem
{
	Q_DECLARE_TR_FUNCTIONS(Node)

public:
	Node();
	~Node();

	void setText(const QString &text);
	QString text() const;
	void setOutlineColor(const QColor &outlineColor);
	QColor outlineColor() const;
	void setBackgroundColor(const QColor &backgroundColor);
	QColor backgroundColor() const;
	void setTextColor(const QColor &textColor);
	QColor textColor() const;
	QList<Link*> links();

	void addLink(Link* link);
	bool removeLink(Link* link);

	QRectF boundingRect() const;
	//QPainterPath shape() const;
	QRectF outlineRect() const;
	void paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QColor myOutlineColor;
	QColor myBackgroundColor;
	QColor myTextColor;
	QString myText;
	QList<Link*> myLinks;
};

#endif // NODE_H
