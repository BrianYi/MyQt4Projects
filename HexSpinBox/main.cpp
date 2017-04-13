#include "hexspinbox.h"
#include <QtGui/QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWin;
    HexSpinBox hexSpinBox(&mainWin);
    mainWin.setCentralWidget(&hexSpinBox);
    mainWin.show();
    return a.exec();
}
