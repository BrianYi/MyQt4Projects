#include "puzzleblock.h"

PuzzleBlock::PuzzleBlock(const QPixmap& pixmap, const qint32& width, const qint32& height)
{
    myPixmap = pixmap;
    myWith = width;
    myHeight = height;
}

PuzzleBlock::~PuzzleBlock()
{

}

QRectF PuzzleBlock::boundingRect() const
{
    return QRectF( -(myWith / 2), -(myHeight / 2), myWith, myHeight);
}

void PuzzleBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
    painter->drawPixmap(boundingRect().toRect(), myPixmap);
}
