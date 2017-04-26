#include "puzzleblock.h"

PuzzleBlock::PuzzleBlock(const QPixmap& pixmap, const qint32& width, const qint32& height)
{
    myPixmap = pixmap;
    myWith = width;
    myHeight = height;
	setFlags(ItemIsSelectable);
}

PuzzleBlock::~PuzzleBlock()
{

}

QRectF PuzzleBlock::boundingRect() const
{
    return QRectF( 0/*-(myWith / 2)*/, 0/*-(myHeight / 2)*/, myWith, myHeight);
}

void PuzzleBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	QRect rect = boundingRect().toRect();
	if (option->state & QStyle::State_Selected) {
		QPen pen(Qt::darkRed);
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
		painter->setPen(pen);
		painter->drawRect(rect.adjusted(+1, +1, -1, -1));
	}
    painter->drawPixmap(rect.adjusted(+2, +2, -2, -2), myPixmap);

}

QPixmap PuzzleBlock::pixmap()
{
	return myPixmap;
}
