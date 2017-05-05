#ifndef LOCALDIRWIDGET_H
#define LOCALDIRWIDGET_H

#include <QtGui>
#include "dirtablemodel.h"

class LocalDirWidget : public QWidget
{
	Q_OBJECT

public:
	LocalDirWidget(QWidget *parent = 0);
	~LocalDirWidget();

private:
	DirTableModel *localDirTableModel;
	QTableView *localDirTableView;
	QFileSystemModel *localDirFileSystemModel;
	QComboBox *localDirComboBox;
	QToolButton *dotdotDirToolButton;
	QToolButton *refreshDirToolButton;
	QStatusBar *localDirStatusBar;
};

#endif // LOCALDIRWIDGET_H
