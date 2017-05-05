#include "remotedirwidget.h"

RemoteDirWidget::RemoteDirWidget(QWidget *parent)
	: QWidget(parent)
{
	remoteDirTableModel = new DirTableModel(this);

	remoteDirTableView = new QTableView(this);
	remoteDirTableView->setModel(remoteDirTableModel);
	remoteDirTableView->setAlternatingRowColors(true);
	remoteDirTableView->horizontalHeader()->setStretchLastSection(true);
	remoteDirTableView->resizeColumnsToContents();
	remoteDirTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	remoteDirTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	remoteDirTableView->setShowGrid(false);
	remoteDirTableView->setSortingEnabled(true);
	remoteDirTableView->sortByColumn(0, Qt::AscendingOrder);

	remoteDirFileSystemModel = new QFileSystemModel(this);
	remoteDirComboBox = new QComboBox(this);

	dotdotDirToolButton = new QToolButton(this);
	dotdotDirToolButton->setText(tr("上级目录"));
	refreshDirToolButton = new QToolButton(this);
	refreshDirToolButton->setText(tr("刷新"));

	logTextEdit = new QTextEdit(this);
	logTextEdit->setFixedHeight(100);
	logTextEdit->setReadOnly(true);
	remoteDirStatusBar = new QStatusBar(this);

	QHBoxLayout *topHBoxLayout = new QHBoxLayout;
	topHBoxLayout->addWidget(dotdotDirToolButton);
	topHBoxLayout->addWidget(refreshDirToolButton);
	topHBoxLayout->addWidget(remoteDirComboBox);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topHBoxLayout);
	mainLayout->addWidget(remoteDirTableView);
	mainLayout->addWidget(logTextEdit);
	mainLayout->addWidget(remoteDirStatusBar);
	mainLayout->setStretch(1,1);
	setLayout(mainLayout);

	setWindowTitle(tr("本地"));

	connect(remoteDirTableView, SIGNAL(doubleClicked(const QModelIndex &)), remoteDirTableModel, SLOT(setRootIndex(const QModelIndex &)));
}

RemoteDirWidget::~RemoteDirWidget()
{

}
