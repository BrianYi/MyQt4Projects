#ifndef REMOTEDIRWIDGET_H
#define REMOTEDIRWIDGET_H

#include <QtGui>
#include "dirtablemodel.h"

class RemoteDirWidget : public QWidget
{
	Q_OBJECT

public:
	RemoteDirWidget(QWidget *parent = 0);
	~RemoteDirWidget();

private:
	DirTableModel *remoteDirTableModel;
	QTableView *remoteDirTableView;
	QFileSystemModel *remoteDirFileSystemModel;
	QComboBox *remoteDirComboBox;
	QToolButton *dotdotDirToolButton;
	QToolButton *refreshDirToolButton;
	QStatusBar *remoteDirStatusBar;
	QTextEdit *logTextEdit;
	QSplitter *splitter;
};

#endif // REMOTEDIRWIDGET_H
