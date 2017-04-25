#include "puzzlegame.h"

PuzzleGame::PuzzleGame(QWidget *parent)
    : QMainWindow(parent)
{
    puzzleScene = new QGraphicsScene(0, 0, PuzzleWidth, PuzzleHeight, this);

    puzzleView = new QGraphicsView(puzzleScene, this);

    setCentralWidget(puzzleView);

    myRow = PuzzleRow;
    myCol = PuzzleCol;

    QPixmap pixmap(tr(":images/puzzlemap.jpg"));
    myPuzzleMap = pixmap;

    generatePuzzleBlocks();
}

PuzzleGame::~PuzzleGame()
{

}

void PuzzleGame::generatePuzzleBlocks()
{
    qint32 blockCount = myRow * myCol;
    QList<BlockPos> blockPos;
    for (int row = 0; row < myRow; row++) {
        for (int col = 0; col < myCol; col++) {
            blockPos << BlockPos(row, col);
        }
    }
    std::random_shuffle(blockPos.begin(), blockPos.end());
    
    int blockWidth = PuzzleWidth / myCol;
    int blockHeight = PuzzleHeight / myRow;
    int i = 0;
    for (auto it = blockPos.begin(); it != blockPos.end(); it++) {
        QRect rect(it->first * blockWidth, it->second * blockHeight, blockWidth, blockHeight);
        QPixmap pixmap = myPuzzleMap.copy(rect);
        PuzzleBlock *puzzleBlock = new PuzzleBlock(pixmap, blockWidth, blockHeight);
        puzzleBlock->setPos(blockWidth * (i % myCol), blockHeight * (i / myRow));
        puzzleScene->addItem(puzzleBlock);
        puzzleBlocks.append(puzzleBlock);
        i++;
    }
}

void PuzzleGame::setPuzzleMap(const QPixmap &puzzleMap)
{
    myPuzzleMap = puzzleMap;
}
