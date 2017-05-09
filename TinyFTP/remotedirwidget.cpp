#include "remotedirwidget.h"
#include "logthread.h"
#include "tinyftp.h"

RemoteDirWidget::RemoteDirWidget(QWidget *parent)
	: QWidget(parent)
{
	parentTinyFtp = reinterpret_cast<TinyFTP*>(parent);

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
	remoteDirFileSystemModel->setFilter(QDir::AllDirs | QDir::Drives | 
		QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot);

	remoteDirComboTreeView = new QTreeView;
	remoteDirComboTreeView->setModel(remoteDirFileSystemModel);
	remoteDirComboTreeView->resizeColumnToContents(0);
	remoteDirComboTreeView->hideColumn(1);
	remoteDirComboTreeView->hideColumn(2);
	remoteDirComboTreeView->hideColumn(3);
	remoteDirComboTreeView->setHeaderHidden(true);
	remoteDirComboTreeView->setItemsExpandable(true);

	remoteDirComboBox = new QComboBox(this);
	remoteDirComboBox->setEnabled(false);
// 	remoteDirComboBox->setModel(remoteDirFileSystemModel);
// 	remoteDirComboBox->setView(remoteDirComboTreeView);

	connectButton = new QToolButton(this);
	connectButton->setText(tr("断开"));
	connectButton->setEnabled(false);
	dotdotDirToolButton = new QToolButton(this);
	dotdotDirToolButton->setText(tr("上级目录"));
	dotdotDirToolButton->setEnabled(false);
	refreshDirToolButton = new QToolButton(this);
	refreshDirToolButton->setText(tr("刷新"));
	refreshDirToolButton->setEnabled(false);

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
	topHBoxLayout->addWidget(connectButton);
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

	//*******************************
	// treeview context menu
	contextMenu = new QMenu(this);
	downloadAction = new QAction(tr("下载"), this);
	queueAction = new QAction(tr("队列"), this);
	sendToAction = contextMenu->addMenu(new QMenu(tr("发送到"), this));
	editAction = new QAction(tr("编辑"), this);
	readAction = new QAction(tr("查看"), this);
	changePermissionAction = new QAction(tr("更改文件权限"), this);
	delAction = new QAction(tr("删除"), this);
	renameAction = new QAction(tr("重命名"), this);
	propertyAction = new QAction(tr("属性"), this);
	contextMenu->addAction(downloadAction);
	contextMenu->addAction(queueAction);
	contextMenu->addAction(sendToAction);
	contextMenu->addAction(editAction);
	contextMenu->addAction(readAction);
	contextMenu->addAction(changePermissionAction);
	contextMenu->addAction(delAction);
	contextMenu->addAction(renameAction);
	contextMenu->addAction(propertyAction);

	setWindowTitle(tr("本地"));

/*	connect(remoteDirTableView, SIGNAL(doubleClicked(const QModelIndex &)), remoteDirTableModel, SLOT(setRootIndex(const QModelIndex &)));*/
    connect(remoteDirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), 
		this, SLOT(setRootIndex(const QModelIndex &)));
    connect(ftpClient, SIGNAL(listInfo(const QUrlInfo &)), 
		this, SLOT(ftpListInfo(const QUrlInfo &)));
    connect(ftpClient, SIGNAL(done(bool)), 
		this, SLOT(ftpDone(bool)));
    connect(ftpClient, SIGNAL(commandFinished(int,bool)), 
		this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftpClient, SIGNAL(commandStarted(int)), 
		this, SLOT(ftpCommandStarted(int)));
	connect(remoteDirTreeView, SIGNAL(pressed(const QModelIndex &)), 
		this, SLOT(showContextMenu(const QModelIndex &)));
	connect(this, SIGNAL(ftpCommandDone(QFtp::Command, bool)), 
		parentTinyFtp, SLOT(ftpCommandDone(QFtp::Command, bool)));
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
    logTextEdit->append("[" + QDateTime::currentDateTime().toString("hh:mm:ss")  + "] " + logData);
}

bool RemoteDirWidget::connectToHost(const QString &address, const QString &port, 
    const QString &username/* = QString()*/, const QString &password/* = QString()*/)
{
    QString strAddress = address;
//     if (!strAddress.startsWith(tr("ftp://"), Qt::CaseInsensitive)) {
//         strAddress = tr("ftp://") + strAddress;
//     }

    urlAddress.setHost(strAddress);
	urlAddress.setPort(port.toInt());
	urlAddress.setUserName(username);
	urlAddress.setPassword(password);

	connectButton->setText(tr("连接"));
	connectButton->setEnabled(false);
	dotdotDirToolButton->setEnabled(false);
	refreshDirToolButton->setEnabled(false);
	remoteDirComboBox->setEnabled(false);
//     if (!urlAddress.isValid()) {
//         writeLog(tr("Error: Invalid URL"));
//         return false;
//     }
//     if (url.scheme() != "ftp") {
//          LOGSTREAM << DataPair(this, tr("Error: URL must start with 'ftp:'"));
//          return false;
//     }
	if (ftpClient->state() != QFtp::Unconnected) {
		ftpClient->close();
	}
    ftpClient->connectToHost(urlAddress.host(), urlAddress.port());
    

//     QString path = url.path();
//     if (path.isEmpty())
//         path = QDir::separator();
// 
// /*    pendingDirs.append(path);*/
// 
//     processDirectory(path);

    return true;
}

void RemoteDirWidget::closeEvent(QCloseEvent *event)
{
	if (ftpClient->state() != QFtp::Unconnected)
		ftpClient->close();
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
	if (currentDir.isEmpty()) {
		return ;
	}
    if (error) {
        writeLog(tr("Error: ") + ftpClient->errorString());
		return ;
	} 
// 	else {
// 		writeLog(tr("Downloaded ") + currentDir + tr(" to ") +
// 			QDir::toNativeSeparators(QDir(currentLocalDir).canonicalPath()));
// 	}
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

void RemoteDirWidget::showContextMenu(const QModelIndex &index)
{
	if (QApplication::mouseButtons() == Qt::RightButton) {
		QList<QAction*> actions = contextMenu->actions();
		foreach (QAction* action, actions)
			action->setEnabled(true);
		Node *node = static_cast<Node*>(index.internalPointer());
		QFileInfo fileInfo(node->path);
		if (fileInfo.isDir() && node->fileName == tr("..")) {
			foreach (QAction* action, actions)
				action->setEnabled(false);
		} else if (true/*fileInfo.isFile()*/) {
			if (!fileInfo.isWritable()) {
				editAction->setEnabled(false);
				delAction->setEnabled(false);
				renameAction->setEnabled(false);
			}
			if (!fileInfo.isReadable()) {
				readAction->setEnabled(false);
				downloadAction->setEnabled(false);
				queueAction->setEnabled(false);
			}
		}

		contextMenu->exec(QCursor::pos());
	}
}

void RemoteDirWidget::ftpCommandStarted(int)
{
	QFtp::Command command = ftpClient->currentCommand();
    if (command == QFtp::ConnectToHost) {
        writeLog(tr("正在解析地址 %1, 端口 %2").arg(
			urlAddress.host()).arg(urlAddress.port()));
    } else if (command == QFtp::Login) {
		if (urlAddress.userName().isEmpty()) {
			writeLog(tr("正在连接到服务器...(%1)").arg(tr("匿名")));
		} else {
			writeLog(tr("正在连接到服务器...(USER %1, PASS (%2))").arg(
			urlAddress.userName()).arg(tr("隐藏")));
		}
    } else if (command == QFtp::Close) {
    } else if (command == QFtp::List) {
    } else if (command == QFtp::Cd) {
    } else if (command == QFtp::Get) {
    } else if (command == QFtp::Put) {
    } else if (command == QFtp::Remove) {
    } else if (command == QFtp::Mkdir) {
    } else if (command == QFtp::Rmdir) {
    } else if (command == QFtp::Rename) {
    }
}

void RemoteDirWidget::ftpCommandFinished(int,bool error)
{
	QFtp::Command command = ftpClient->currentCommand();
    if (command == QFtp::ConnectToHost) {
        if (!error) {
            writeLog(tr("已连接到服务器, 正在等待响应..."));
			ftpClient->login(urlAddress.userName(), urlAddress.password());
        } else {
            writeLog(tr("不能连接到服务器(%1 无法解析)").arg(urlAddress.host()));
			if (ftpClient->state() != QFtp::Unconnected) {
				ftpClient->close();
			}
        }
    } else if (command == QFtp::Login) {
        if (!error) {
            writeLog(tr("连接成功(登录成功)"));
			QString path = urlAddress.path();
			if (path.isEmpty()) {
				path = QDir::separator();
			}
			processDirectory(path);

			connectButton->setText(tr("断开"));
			connectButton->setEnabled(true);
			dotdotDirToolButton->setEnabled(true);
			refreshDirToolButton->setEnabled(true);
			remoteDirComboBox->setEnabled(true);

			//*******************************
			// 设置ComboBox目录树显示
// 			remoteDirFileSystemModel->setRootPath()
// 			remoteDirComboBox->setModel(remoteDirFileSystemModel);
// 			remoteDirComboBox->setView(remoteDirComboTreeView);
        } else {
            writeLog(tr("连接失败(登录失败), 用户名或密码错误!")/* + ftpClient->errorString()*/);
			if (ftpClient->state() != QFtp::Unconnected) {
				ftpClient->close();
			}
        }
    } else if (command == QFtp::Close) {
        if (!error) {
            writeLog(tr("已从服务器断开连接"));
        } else {
            writeLog(tr("无法断开连接, ") + ftpClient->errorString());
        }
    } else if (command == QFtp::List) {
    } else if (command == QFtp::Cd) {
    } else if (command == QFtp::Get) {
    } else if (command == QFtp::Put) {
    } else if (command == QFtp::Remove) {
    } else if (command == QFtp::Mkdir) {
    } else if (command == QFtp::Rmdir) {
    } else if (command == QFtp::Rename) {
    }

	emit ftpCommandDone(command, error);
}
