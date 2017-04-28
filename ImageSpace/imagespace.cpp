#include <QtGui>
#include <iostream>

qint64 imageSpace(QString path)
{
	qint64 size = 0;
	QDir dir(path);
	QStringList filter;
	foreach (QByteArray format, QImageReader::supportedImageFormats())
		filter += "*." + format;

	foreach (QString file, dir.entryList(filter, QDir::Files))
		size += QFileInfo(dir, file).size();

	foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
		size += imageSpace(path + QDir::separator() + subDir);
	return size;
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QStringList args = QCoreApplication::arguments();

	QString path = QDir::currentPath();
	if (args.count() > 1) {
		path = args[1];
	}

	std::cout << "Space used by images in " << qPrintable(path)
			  << " and its subdirectories is "
			  << (imageSpace(path) / 1024) << " KB" << std::endl;

	return 0;
}