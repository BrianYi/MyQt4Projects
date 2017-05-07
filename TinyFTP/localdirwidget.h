#ifndef LOCALDIRWIDGET_H
#define LOCALDIRWIDGET_H

#include <QtGui>
#include "dirtreemodel.h"

class LocalDirWidget : public QWidget
{
	Q_OBJECT

public:
	LocalDirWidget(QWidget *parent = 0);
	~LocalDirWidget();
private:
	DirTreeModel *localDirTreeModel;
	QTreeView *localDirTreeView;
	QFileSystemModel *localDirFileSystemModel;
	QComboBox *localDirComboBox;
	QToolButton *dotdotDirToolButton;
	QToolButton *refreshDirToolButton;
	QStatusBar *localDirStatusBar;
};

#endif // LOCALDIRWIDGET_H
