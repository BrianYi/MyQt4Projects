#include "mydiagram.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyDiagram w;
	w.show();
	return a.exec();
}
