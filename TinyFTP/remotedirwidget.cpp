#include "remotedirwidget.h"
#include "logthread.h"

RemoteDirWidget::RemoteDirWidget(QWidget *parent)
	: QWidget(parent)
{
	remoteDirTreeModel = new DirTreeModel(this);

	remoteDirTreeView = new QTreeView(this);
    remoteDirTreeView->setModel(remoteDirTreeModel);
    remoteDirTreeView->header()->setStretchLastSection(true);   
    remoteDirTreeView->resizeColumnToContents(0);
	remoteDirTreeView->setAlternatingRowColors(true);
	remoteDirTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
	remoteDirTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	remoteDirTreeView->setSortingEnabled(true);
	remoteDirTreeView->sortByColumn(0, Qt::AscendingOrder);
    remoteDirTreeView->setItemsExpandable(false);
    remoteDirTreeView->setRootIsDecorated(false);

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
	mainLayout->addWidget(remoteDirTreeView);
	mainLayout->addWidget(logTextEdit);
	mainLayout->addWidget(remoteDirStatusBar);
	mainLayout->setStretch(1,1);
	setLayout(mainLayout);

	setWindowTitle(tr("本地"));

/*	connect(remoteDirTableView, SIGNAL(doubleClicked(const QModelIndex &)), remoteDirTableModel, SLOT(setRootIndex(const QModelIndex &)));*/
    connect(remoteDirTreeView, SIGNAL(itemDoubleClicked(QTreeWidgetItem *)), this, SLOT(itemDoubleClicked(QTreeWidgetItem *)));
    connect(ftpClient, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(ftpListInfo(const QUrlInfo &)));
    connect(ftpClient, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
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

bool RemoteDirWidget::getDirectory(const QString &address, const QString &port, 
    const QString &username/* = QString()*/, const QString &password/* = QString()*/)
{
    QString urlAddress = address;
    if (!urlAddress.startsWith(tr("ftp://"), Qt::CaseInsensitive)) {
        urlAddress = tr("ftp://") + urlAddress;
    }

    QUrl url(urlAddress);
    if (!url.isValid()) {
        LOGSTREAM << DataPair(this, tr("Error: Invalid URL"));
        return false;
    }

//     if (url.scheme() != "ftp") {
//          LOGSTREAM << DataPair(this, tr("Error: URL must start with 'ftp:'"));
//          return false;
//     }

    ftpClient->connectToHost(url.host(), url.port(21));
    ftpClient->login(username, password);

    QString path = url.path();
    if (path.isEmpty())
        path = "/";

/*    pendingDirs.append(path);*/

    processDirectory(path);

    return true;
}

void RemoteDirWidget::ftpListInfo(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()) {
        if (urlInfo.isReadable()) {
            QString path = currentLocalDir + tr("/")
                + QString::fromUtf8(urlInfo.name().toLatin1());
            QFile file(path);
            if (file.exists()) {
                return ;
            } else if (!file.open(QIODevice::WriteOnly)) {
                LOGSTREAM << DataPair(this, tr("Warning: Cannot write file ") +
                    QDir::toNativeSeparators(
                    file.fileName()) +
                    ": " + file.errorString());
                return ;
            }
        }
    } else if (urlInfo.isDir() && !urlInfo.isSymLink()) {
        //pendingDirs.append(currentDir + "/" + urlInfo.name());
        QString localDir = currentLocalDir + QString::fromUtf8(urlInfo.name().toLatin1());
        QDir(".").mkpath(localDir);
    }
}


void RemoteDirWidget::itemDoubleClicked(QTreeWidgetItem *item)
{

}


void RemoteDirWidget::processDirectory(const QString &dir)
{
    currentDir = QString::fromUtf8(dir.toLatin1());
    currentLocalDir = tr("cache/") + dir;
    QDir(".").mkpath(currentLocalDir);
    ftpClient->cd(currentDir);
    ftpClient->list();
}

void RemoteDirWidget::ftpDone(bool error)
{
    if (error) {
        LOGSTREAM << DataPair(this, tr("Error: ") + ftpClient->errorString());
    } else {
        LOGSTREAM << DataPair(this, tr("Downloaded ") + currentDir + tr(" to ") +
            QDir::toNativeSeparators(QDir(currentLocalDir).canonicalPath()));
    }
    remoteDirTreeModel->setRootPath(currentLocalDir);
   
    if (remoteDirTreeModel->rowCount() > 1) {
        for (int row = 1; row <= remoteDirTreeModel->rowCount(); row++) {
            remoteDirTreeModel->setData(remoteDirTreeModel->index(row, 1), 5);
        }
    }
}
