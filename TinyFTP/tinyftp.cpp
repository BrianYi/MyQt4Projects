#include "tinyftp.h"

TinyFTP::TinyFTP(QWidget *parent)
	: QMainWindow(parent)
{
	splitter = new QSplitter(this);
	localDirTableModel = new TableModel(this);
	localDirTableModel->setRootPath(QDir::currentPath());

	localDirTableView = new QTableView(this);
	localDirTableView->setModel(localDirTableModel);
	localDirTableView->setAlternatingRowColors(true);
    localDirTableView->horizontalHeader()->setStretchLastSection(true);
    localDirTableView->resizeColumnsToContents();
    localDirTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    localDirTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	splitter->addWidget(localDirTableView);
	setCentralWidget(splitter);
}

TinyFTP::~TinyFTP()
{

}
