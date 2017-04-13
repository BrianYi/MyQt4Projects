#include "myplotter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPlotter w;
    w.show();
    return a.exec();
}
