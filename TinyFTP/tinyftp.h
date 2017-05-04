#ifndef TINYFTP_H
#define TINYFTP_H

#include <QWidget>
#include <QtGui>
#include "filesystemmodel.h"

class TinyFTP : public QMainWindow
{
	Q_OBJECT

public:
	TinyFTP(QWidget *parent = 0);
	~TinyFTP();

private:
	QSplitter *splitter;
	FileSystemModel *localFileSystemModel;
	QTreeView *localTreeView;
};

#endif // TINYFTP_H
