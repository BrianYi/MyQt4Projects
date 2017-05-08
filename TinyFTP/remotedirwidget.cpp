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
	dotdotDirToolButton->setText(tr("�ϼ�Ŀ¼"));
	refreshDirToolButton = new QToolButton(this);
	refreshDirToolButton->setText(tr("ˢ��"));

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

	setWindowTitle(tr("����"));

/*	connect(remoteDirTableView, SIGNAL(doubleClicked(const QModelIndex &)), remoteDirTableModel, SLOT(setRootIndex(const QModelIndex &)));*/
    connect(remoteDirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(setRootIndex(const QModelIndex &)));
    connect(ftpClient, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(ftpListInfo(const QUrlInfo &)));
    connect(ftpClient, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    connect(ftpClient, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftpClient, SIGNAL(commandStarted(int)), this, SLOT(ftpCommandStarted(int)));
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
        writeLog(tr("Error: Invalid URL"));
        return false;
    }

//     if (url.scheme() != "ftp") {
//          LOGSTREAM << DataPair(this, tr("Error: URL must start with 'ftp:'"));
//          return false;
//     }

    ftpClient->connectToHost(url.host(), port.toUShort());
    ftpClient->login(username, password);

    QString path = url.path();
    if (path.isEmpty())
        path = QDir::separator();

/*    pendingDirs.append(path);*/

    processDirectory(path);

    return true;
}

void RemoteDirWidget::ftpListInfo(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()) {
        if (urlInfo.isReadable()) {
			filesSize.push_back(urlInfo.size());
            filesModifyDate.append(urlInfo.lastModified().toString("yyyy/MM/dd hh:mm"));
            QString path = currentLocalDir + QDir::separator()
                + ftpClient->decoded(urlInfo.name());
            QFile file(path);
            if (file.exists()) {
                return ;
            } else if (!file.open(QIODevice::WriteOnly)) {
                writeLog(tr("Warning: Cannot write file ") +
                    QDir::toNativeSeparators(
                    file.fileName()) +
                    ": " + file.errorString());
                return ;
            }
        }
    } else if (urlInfo.isDir() && !urlInfo.isSymLink()) {
        //pendingDirs.append(currentDir + "/" + urlInfo.name());
        filesModifyDate.append(urlInfo.lastModified().toString("yyyy/MM/dd hh:mm"));
        QString localDir = currentLocalDir + ftpClient->decoded(urlInfo.name());
        QDir(".").mkpath(localDir);
        return ;
    }
}

void RemoteDirWidget::processDirectory(const QString &dir)
{
    currentDir = dir;
    currentLocalDir = tr("cache") + dir;
    QDir(".").mkpath(currentLocalDir);
    //ftpClient->cd(currentDir);
    ftpClient->list(currentDir);
}

void RemoteDirWidget::ftpDone(bool error)
{
    if (error) {
        writeLog(tr("Error: ") + ftpClient->errorString());
    } else {
        writeLog(tr("Downloaded ") + currentDir + tr(" to ") +
            QDir::toNativeSeparators(QDir(currentLocalDir).canonicalPath()));
    }
    remoteDirTreeModel->setRootPath(currentLocalDir);
	remoteDirTreeView->resizeColumnToContents(0);
    if (remoteDirTreeModel->rowCount()) {
        for (int row = 0; row < remoteDirTreeModel->rowCount(); row++) {
			QModelIndex index = remoteDirTreeModel->index(row, 1);
            QModelIndex index2 = remoteDirTreeModel->index(row, 3);
			Node *node = static_cast<Node*>(index.internalPointer());
			if (node->isFile) {
                remoteDirTreeModel->setData(index, filesSize.takeFirst());
			}
            if (node->isDir && node->fileName == tr("..")) {
                remoteDirTreeModel->setData(index2, "");
            } else
                remoteDirTreeModel->setData(index2, filesModifyDate.takeFirst());
        }
    }

	if (!filesSize.isEmpty()) {
		writeLog(tr("Error: file sizes is not correspond with files"));
	} else if (!filesModifyDate.isEmpty()) {
        writeLog(tr("Error: file modify date is not correspond with files"));
    }
}

void RemoteDirWidget::setRootIndex(const QModelIndex &index)
{
	if (!index.isValid()) {
		return ;
	}
	Node *node = static_cast<Node*>(index.internalPointer());
	if (node->isDir) {
        if (node->fileName == tr("..")) {
            if (!currentDir.endsWith(QDir::separator()) || currentDir == QDir::separator())
                return ;
            currentDir.resize(currentDir.length() - 1);
            int lastSeparatorIndex = currentDir.lastIndexOf(QDir::separator());
            if (lastSeparatorIndex != -1)
                currentDir = currentDir.mid(0, lastSeparatorIndex + 1);
        } else
		    currentDir += node->fileName + QDir::separator();	
        processDirectory(currentDir);
	}
	/*remoteDirTreeModel->set(index);*/
// 	remoteDirTreeView->resizeColumnToContents(0);
}

void RemoteDirWidget::ftpCommandStarted(int)
{
    if (ftpClient->currentCommand() == QFtp::ConnectToHost) {
        writeLog(tr("���ڽ�����ַ"));
    } else if (ftpClient->currentCommand() == QFtp::Login) {
        writeLog(tr("�������ӵ�"));
    } else if (ftpClient->currentCommand() == QFtp::Close) {
    } else if (ftpClient->currentCommand() == QFtp::List) {
    } else if (ftpClient->currentCommand() == QFtp::Cd) {
    } else if (ftpClient->currentCommand() == QFtp::Get) {
    } else if (ftpClient->currentCommand() == QFtp::Put) {
    } else if (ftpClient->currentCommand() == QFtp::Remove) {
    } else if (ftpClient->currentCommand() == QFtp::Mkdir) {
    } else if (ftpClient->currentCommand() == QFtp::Rmdir) {
    } else if (ftpClient->currentCommand() == QFtp::Rename) {
    }
}

void RemoteDirWidget::ftpCommandFinished(int,bool error)
{
    if (ftpClient->currentCommand() == QFtp::ConnectToHost) {
        if (!error) {
            writeLog(tr("�������"));
        } else {
            writeLog(tr("����ʧ�ܣ�") + ftpClient->errorString());
        }
    } else if (ftpClient->currentCommand() == QFtp::Login) {
        if (!error) {
            writeLog(tr("�����ӵ������������ڵȴ���Ӧ..."));
        } else {
            writeLog(tr("���ӷ�����ʧ�ܣ�") + ftpClient->errorString());
        }
    } else if (ftpClient->currentCommand() == QFtp::Close) {
        if (!error) {
            writeLog(tr("�Ѵӷ������Ͽ�"));
        } else {
            writeLog(tr("�޷��Ͽ����ӣ�") + ftpClient->errorString());
        }
    } else if (ftpClient->currentCommand() == QFtp::List) {
    } else if (ftpClient->currentCommand() == QFtp::Cd) {
    } else if (ftpClient->currentCommand() == QFtp::Get) {
    } else if (ftpClient->currentCommand() == QFtp::Put) {
    } else if (ftpClient->currentCommand() == QFtp::Remove) {
    } else if (ftpClient->currentCommand() == QFtp::Mkdir) {
    } else if (ftpClient->currentCommand() == QFtp::Rmdir) {
    } else if (ftpClient->currentCommand() == QFtp::Rename) {
    }
}
