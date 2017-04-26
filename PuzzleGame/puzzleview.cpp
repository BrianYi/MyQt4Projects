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
	QList<QGraphicsItem*> items = scene()->selectedItems();
	if (items.count() == 1) {
		return dynamic_cast<PuzzleBlock *>(items.first());
	} else {
		return 0;
	}
}

void PuzzleView::mouseMoveEvent(QMouseEvent *event)
{
	PuzzleBlock *block = selectedBlock();
	if ((event->buttons() & Qt::LeftButton) && block) {
		int distance = (event->pos() - startPos).manhattanLength();
		if (distance >= QApplication::startDragDistance()) {
			QMimeData *mimeData = new QMimeData;
			QPixmap pix = block->pixmap();
			mimeData->setImageData(pix);

			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			drag->setPixmap(pix);
			drag->exec(Qt::MoveAction);
		}
	}
	QGraphicsView::mouseMoveEvent(event);
}

void PuzzleView::dragEnterEvent(QDragEnterEvent *event)
{

	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void PuzzleView::dragMoveEvent(QDragMoveEvent *event)
{

	event->setDropAction(Qt::MoveAction);
	event->accept();

}

void PuzzleView::dropEvent(QDropEvent *event)
{

}

void PuzzleView::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		startPos = event->pos();
	}
	QGraphicsView::mousePressEvent(event);
}
