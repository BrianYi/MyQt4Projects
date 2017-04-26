#include "puzzleview.h"


PuzzleView::PuzzleView(QWidget *parent)
	: QGraphicsView(parent)
{
	
}

PuzzleView::~PuzzleView()
{

}

PuzzleBlock *PuzzleView::selectedBlock() const
{
	return dynamic_cast<PuzzleBlock *>(scene()->focusItem());
}

void PuzzleView::mouseMoveEvent(QMouseEvent *event)
{
	PuzzleBlock *block = selectedBlock();
	if ((event->buttons() & Qt::LeftButton) && block) {
		int distance = (event->pos() - startPos).manhattanLength();

		if (distance >= QApplication::startDragDistance()) {
			block->enablePixmap(false);

			QMimeData *mimeData = new QMimeData;
			QPixmap pix = block->pixmap();
			mimeData->setImageData(pix);

			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			drag->setPixmap(pix);
			drag->setHotSpot((event->pos() - block->pos()).toPoint());
			drag->exec(Qt::MoveAction);
		}
	}
	QGraphicsView::mouseMoveEvent(event);
}

void PuzzleView::dragEnterEvent(QDragEnterEvent *event)
{
	PuzzleBlock *block = selectedBlock();
	if (block) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
}

void PuzzleView::dragMoveEvent(QDragMoveEvent *event)
{
	PuzzleBlock *block = selectedBlock();
	if (block) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
}

void PuzzleView::dropEvent(QDropEvent *event)
{
	PuzzleBlock *block = selectedBlock();
	PuzzleBlock *newBlock = dynamic_cast<PuzzleBlock *>(itemAt(event->pos()));
	if (block && newBlock) {
		if (block != newBlock) {
			QPointF pos1 = block->pos();
			QPointF pos2 = newBlock->pos();
			block->setPos(pos2);
			newBlock->setPos(pos1);
		}
	}

	if (block)
		block->enablePixmap(true);
}

void PuzzleView::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		startPos = event->pos();
	}
	QGraphicsView::mousePressEvent(event);
}
