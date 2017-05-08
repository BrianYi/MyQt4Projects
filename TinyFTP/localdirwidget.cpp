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

	localDirFileSystemModel = new DirTreeModel(this);
	localDirFileSystemModel->setRootPath(QDir::currentPath());
	//QModelIndex index = localDirFileSystemModel->setRootPath(QDir::currentPath());
	localDirComboBox = new QComboBox(this);
	localDirComboBox->setModel(localDirFileSystemModel);
	//localDirComboBox->setRootModelIndex(index);

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
		/*localDirTreeModel->setRootIndex(index);*/
		if (node->children.isEmpty()) {
			QFileIconProvider provider;
			foreach (QFileInfo fileInfo, QDir(node->path).entryInfoList(QDir::NoDot | QDir::AllEntries, 
				QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)) {
					Node *p = new Node;
					p->fileName = fileInfo.fileName();
					p->fileIcon = provider.icon(fileInfo);
					p->fileSize = fileInfo.size();
					p->fileType = provider.type(fileInfo);
					p->modifyDate = fileInfo.lastModified().toString("yyyy/MM/dd hh:mm");
					p->isDir = fileInfo.isDir();
					p->isSystemLink = fileInfo.isSymLink();
					p->isFile = fileInfo.isFile();
					p->path = fileInfo.absoluteFilePath();
					p->parent = node;
					node->children.append(p);
			}
		}
		/*localDirTreeView->setRootIndex(index);*/
		localDirTreeView->reset();
		localDirTreeView->resizeColumnToContents(0);
	}
}
