#ifndef PUZZLEBLOCK_H
#define PUZZLEBLOCK_H

#include <QGraphicsItem>
#include <QtGui>

class PuzzleBlock : public QGraphicsItem
{
public:
    PuzzleBlock(const QPixmap& pixmap, const qint32& width, const qint32& height);
    ~PuzzleBlock();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QPixmap pixmap();
	void enablePixmap(bool enabled);
private:
    QPixmap myPixmap;
    qint32 myWith;
    qint32 myHeight;
	bool myEnablePixmap;
};

#endif // PUZZLEBLOCK_H
