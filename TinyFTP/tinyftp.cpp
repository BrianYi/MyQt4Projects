#include "tinyftp.h"

TinyFTP::TinyFTP(QWidget *parent)
	: QMainWindow(parent)
{
	splitter = new QSplitter(this);
	localFileSystemModel = new FileSystemModel(this);
	localFileSystemModel->setReadOnly(true);
	QModelIndex rootIndex = localFileSystemModel->setRootPath(QDir::currentPath());

	localTreeView = new QTreeView(this);
	localTreeView->setModel(localFileSystemModel);
	localTreeView->setRootIsDecorated(false);
	localTreeView->setRootIndex(rootIndex);
	localTreeView->header()->setStretchLastSection(true);
	localTreeView->resizeColumnToContents(0);
	localTreeView->setExpandsOnDoubleClick(false);

	splitter->addWidget(localTreeView);
	setCentralWidget(localTreeView);
}

TinyFTP::~TinyFTP()
{

}
