#include "puzzlegame.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PuzzleGame w;
    w.show();
    return a.exec();
}
