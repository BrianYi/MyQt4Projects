#include "snakegame.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnakeGame w;
    w.setMapSize(21,21);
    w.begin(SnakeGame::Crazy);
    w.show();
    return a.exec();
}
