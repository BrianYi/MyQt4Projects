#ifndef TINYFTP_H
#define TINYFTP_H

#include <QWidget>
#include <QtGui>
#include "tablemodel.h"

class TinyFTP : public QMainWindow
{
	Q_OBJECT

public:
	TinyFTP(QWidget *parent = 0);
	~TinyFTP();

private:
	QSplitter *splitter;
	TableModel *localDirTableModel;
	QTableView *localDirTableView;
};

#endif // TINYFTP_H
