#ifndef PUZZLEGAME_H
#define PUZZLEGAME_H

#include <QMainWindow>
#include "puzzleblock.h"

class PuzzleGame : public QMainWindow
{
    Q_OBJECT

public:
    enum { 
        PuzzleWidth = 600, 
        PuzzleHeight = 600,
        PuzzleRow = 3,
        PuzzleCol = 3
    };
    PuzzleGame(QWidget *parent = 0);
    ~PuzzleGame();
    void setPuzzleMap(const QPixmap &puzzleMap);
    void setPuzzleSize(const qint32 &row, const qint32 &col);
protected:
    void generatePuzzleBlocks();

private:
    typedef QPair<int, int> BlockPos;
    QGraphicsScene *puzzleScene;
    QGraphicsView *puzzleView;
    QList<PuzzleBlock*> puzzleBlocks;
    QPixmap myPuzzleMap;
    qint32 myRow;
    qint32 myCol;
};

#endif // PUZZLEGAME_H
