#include "localdirwidget.h"

LocalDirWidget::LocalDirWidget(QWidget *parent)
	: QWidget(parent)
{
	localDirTreeModel = new DirTreeModel(this);
	localDirTreeModel->setRootPath(QDir::currentPath());

	localDirTreeView = new QTreeView(this);
	localDirTreeView->setModel(localDirTreeModel);
	localDirTreeView->header()->setStretchLastSection(true);
	localDirTreeView->resizeColumnToContents(0);
    localDirTreeView->setAlternatingRowColors(true);
	localDirTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
	localDirTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	localDirTreeView->setSortingEnabled(true);
	localDirTreeView->sortByColumn(0, Qt::AscendingOrder);
    localDirTreeView->setRootIsDecorated(false);
    localDirTreeView->setItemsExpandable(false);

	localDirFileSystemModel = new QFileSystemModel(this);
	QModelIndex index = localDirFileSystemModel->setRootPath(QDir::currentPath());
	localDirComboBox = new QComboBox(this);
	localDirComboBox->setModel(localDirFileSystemModel);
	localDirComboBox->setRootModelIndex(index);

	dotdotDirToolButton = new QToolButton(this);
	dotdotDirToolButton->setText(tr("�ϼ�Ŀ¼"));
	refreshDirToolButton = new QToolButton(this);
	refreshDirToolButton->setText(tr("ˢ��"));

	localDirStatusBar = new QStatusBar(this);

	QHBoxLayout *topHBoxLayout = new QHBoxLayout;
	topHBoxLayout->addWidget(dotdotDirToolButton);
	topHBoxLayout->addWidget(refreshDirToolButton);
	topHBoxLayout->addWidget(localDirComboBox);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topHBoxLayout);
	mainLayout->addWidget(localDirTreeView);
	mainLayout->addWidget(localDirStatusBar);
	setLayout(mainLayout);

	setWindowTitle(tr("����"));
	connect(localDirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), localDirTreeModel, SLOT(setRootIndex(const QModelIndex &)));
}

LocalDirWidget::~LocalDirWidget()
{
}