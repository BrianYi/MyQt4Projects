#include "preferencedialog.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PreferenceDialog w;
	w.show();
	return a.exec();
}
