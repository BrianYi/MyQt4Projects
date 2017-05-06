#include "remotedirwidget.h"
#include "logthread.h"

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

//     username = tr("");
//     password = tr("");
//     port = 21;
//     address = tr("");
    ftpClient = new FTPClient(this);

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
    connect(ftpClient, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(listInfo(const QUrlInfo &)));
}

RemoteDirWidget::~RemoteDirWidget()
{

}

// void RemoteDirWidget::setLoginInfo(const QString &port, const QString &address, 
//     const QString &usrname/* = QString()*/, const QString &pwd/* = QString()*/)
// {
//     this->username = usrname;
//     this->password = pwd;
//     this->port = port;
//     this->address = address;
// }

void RemoteDirWidget::writeLog(const QString &logData)
{
    logTextEdit->append(logData);
}

bool RemoteDirWidget::getDirectory(QUrl url, const QString &port, const QString &address, 
    const QString &username/* = QString()*/, const QString &password/* = QString()*/)
{
    if (!url.isValid()) {
        LOGSTREAM << DataPair(this, tr("Error: Invalid URL"));
        return false;
    }

    if (url.scheme() != "ftp") {
//         LOGSTREAM << DataPair(this, tr("Error: URL must start with 'ftp:'"));
//         return false;
        url.setHost(tr("ftp://") + url.host());
    }

    ftpClient->connectToHost(url.host(), url.port(21));
    ftpClient->login(username, password);

    QString path = url.path();
    if (path.isEmpty())
        path = "/";

//     pendingDirs.append(path);
//     processNextDirectory();
    ftpClient->cd(path);
    ftpClient->list();

    return true;
}

void RemoteDirWidget::listInfo(const QUrlInfo &urlInfo)
{

}
