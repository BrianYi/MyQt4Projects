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
	localDirFileSystemModel->setFilter(QDir::AllDirs | QDir::Drives | 
		QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot);
	QModelIndex index = localDirFileSystemModel->setRootPath(QDir::currentPath());
	
	localDirComboTreeView = new QTreeView(this);
	localDirComboTreeView->setModel(localDirFileSystemModel);
    localDirComboTreeView->resizeColumnToContents(0);
    localDirComboTreeView->hideColumn(1);
    localDirComboTreeView->hideColumn(2);
    localDirComboTreeView->hideColumn(3);
    localDirComboTreeView->setHeaderHidden(true);
    localDirComboTreeView->expand(index);
    localDirComboTreeView->scrollTo(index);
    localDirComboTreeView->setCurrentIndex(index);
    localDirComboTreeView->setItemsExpandable(true);
	
	localDirComboBox = new QComboBox(this);
	localDirComboBox->setModel(localDirFileSystemModel);
	localDirComboBox->setView(localDirComboTreeView);

	dotdotDirToolButton = new QToolButton(this);
	dotdotDirToolButton->setText(tr("上级目录"));
	refreshDirToolButton = new QToolButton(this);
	refreshDirToolButton->setText(tr("刷新"));

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

	setWindowTitle(tr("本地"));
	connect(localDirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(setRootIndex(const QModelIndex &)));
	/*connect(localDirComboTreeView, SIGNAL())*/
    connect(localDirComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(currentIndexChanged(const QString &)));
}

LocalDirWidget::~LocalDirWidget()
{
}

void LocalDirWidget::setRootIndex(const QModelIndex &index)
{
	if (!index.isValid()) {
		return ;
	}
	Node *node = static_cast<Node*>(index.internalPointer());
	if (node->isDir) {
        QString path = node->path;
		localDirTreeModel->setRootIndex(index);
		localDirTreeView->resizeColumnToContents(0);

        //*******************************
        // 这里的代码没有效果，不知为何
        QModelIndex curIndex = localDirFileSystemModel->index(path);
        localDirComboTreeView->collapseAll();
        localDirComboTreeView->expand(curIndex);
        localDirComboTreeView->reset();
	}
}

void LocalDirWidget::currentIndexChanged(const QString &text)
{
    QModelIndex curIndex = localDirComboTreeView->currentIndex();
    localDirComboTreeView->collapseAll();
    localDirComboTreeView->expand(curIndex);
    localDirComboTreeView->reset();
/*    localDirComboTreeView->scrollTo(curIndex);*/

    localDirTreeModel->setRootPath(localDirFileSystemModel->filePath(curIndex));
    localDirTreeView->resizeColumnToContents(0);
}
