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
	private slots:
		void setRootIndex(const QModelIndex &index);
        void currentIndexChanged(const QString &text);
private:
	DirTreeModel *localDirTreeModel;
	QTreeView *localDirTreeView;
	QFileSystemModel *localDirFileSystemModel;
	QTreeView *localDirComboTreeView;
	QComboBox *localDirComboBox;
	QToolButton *dotdotDirToolButton;
	QToolButton *refreshDirToolButton;
	QStatusBar *localDirStatusBar;
};

#endif // LOCALDIRWIDGET_H
