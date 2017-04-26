#ifndef PUZZLEVIEW_H
#define PUZZLEVIEW_H

#include <QGraphicsView>
#include "puzzleblock.h"

class PuzzleView : public QGraphicsView
{
	Q_OBJECT

public:
	PuzzleView(QWidget *parent = 0);
	~PuzzleView();

protected:
	PuzzleBlock *PuzzleView::selectedBlock() const;
	void mouseMoveEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void mousePressEvent(QMouseEvent *event);
private:
	QPoint startPos;
};

#endif // PUZZLEVIEW_H
