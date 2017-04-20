#include "ticker.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ticker w;
    w.setText("[666666]");
    w.show();
    return a.exec();
}
