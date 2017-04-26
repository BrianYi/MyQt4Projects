#include "puzzleblock.h"

PuzzleBlock::PuzzleBlock(const QPixmap& pixmap, const qint32& width, const qint32& height)
{
    myPixmap = pixmap;
    myWith = width;
    myHeight = height;
	myEnablePixmap = true;
	setFlags(ItemIsFocusable);
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
	if (option->state & QStyle::State_HasFocus) {
		QPen pen(Qt::darkRed);
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
		painter->setPen(pen);
		painter->drawRect(rect.adjusted(+1, +1, -1, -1));
	}

	if (myEnablePixmap)
		painter->drawPixmap(rect.adjusted(+2, +2, -2, -2), myPixmap);
}

QPixmap PuzzleBlock::pixmap()
{
	return myPixmap;
}

void PuzzleBlock::enablePixmap(bool enabled)
{
	myEnablePixmap = enabled;
	update();
}


