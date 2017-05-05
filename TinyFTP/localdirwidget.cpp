#include "localdirwidget.h"

LocalDirWidget::LocalDirWidget(QWidget *parent)
	: QWidget(parent)
{
	localDirTableModel = new DirTableModel(this);
	localDirTableModel->setRootPath(QDir::currentPath());

	localDirTableView = new QTableView(this);
	localDirTableView->setModel(localDirTableModel);
	localDirTableView->setAlternatingRowColors(true);
	localDirTableView->horizontalHeader()->setStretchLastSection(true);
	localDirTableView->resizeColumnsToContents();
	localDirTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	localDirTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	localDirTableView->setShowGrid(false);
	localDirTableView->setSortingEnabled(true);
	localDirTableView->sortByColumn(0, Qt::AscendingOrder);

	localDirFileSystemModel = new QFileSystemModel(this);
	QModelIndex index = localDirFileSystemModel->setRootPath(QDir::currentPath());
	localDirComboBox = new QComboBox(this);
	localDirComboBox->setModel(localDirFileSystemModel);
	localDirComboBox->setRootModelIndex(index);

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
	mainLayout->addWidget(localDirTableView);
	mainLayout->addWidget(localDirStatusBar);
	setLayout(mainLayout);

	setWindowTitle(tr("本地"));

	connect(localDirTableView, SIGNAL(doubleClicked(const QModelIndex &)), localDirTableModel, SLOT(setRootIndex(const QModelIndex &)));
}

LocalDirWidget::~LocalDirWidget()
{

}
