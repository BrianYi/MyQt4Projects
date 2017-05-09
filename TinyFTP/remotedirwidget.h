#ifndef REMOTEDIRWIDGET_H
#define REMOTEDIRWIDGET_H

#include <QtGui>
#include <QFtp>
#include "dirtreemodel.h"
#include "ftpclient.h"

class TinyFTP;
class RemoteDirWidget : public QWidget
{
	Q_OBJECT

public:
	RemoteDirWidget(QWidget *parent);
	~RemoteDirWidget();
//     void setLoginInfo(const QString &port, const QString &address, 
//         const QString &usrname = QString(), const QString &pwd = QString());
    bool connectToHost(const QString &address, const QString &port, const QString &usrname = QString(), 
        const QString &pwd = QString());
protected:
	void closeEvent(QCloseEvent *event);
	private slots:
        void ftpListInfo(const QUrlInfo &urlInfo);
        void ftpDone(bool error);
        void ftpCommandFinished(int,bool error);
        void ftpCommandStarted(int);
		void setRootIndex(const QModelIndex &index);
		void showContextMenu(const QModelIndex &index);
signals:
    void updateLoginInfo(const QString &usrname, 
        const QString &pwd, const QString &port, 
        const QString &address, bool isanonymous);
	void ftpCommandDone(QFtp::Command command, bool error);
private:
	void writeLog(const QString &logData);
    void processDirectory(const QString &dir);
	DirTreeModel *remoteDirTreeModel;
	QTreeView *remoteDirTreeView;
	QFileSystemModel *remoteDirFileSystemModel;
	QTreeView *remoteDirComboTreeView;
	QComboBox *remoteDirComboBox;
	QToolButton *connectButton;
	QToolButton *dotdotDirToolButton;
	QToolButton *refreshDirToolButton;
	QStatusBar *remoteDirStatusBar;
	QTextEdit *logTextEdit;
//     QString username;
//     QString password;
//     QString port;
//     QString address;
	QUrl urlAddress;
    FTPClient *ftpClient;
    QFileIconProvider provider;
    QStringList pendingDirs;
	QQueue<qint64> filesSize;
    QStringList filesModifyDate;
    QString currentDir;
    QString currentLocalDir;
// 	QMenu *tabMenu;
// 	QAction *newTabAction;
// 	QAction *closeTabAction;
// 	QAction *closeOtherTabAction;
	QMenu *contextMenu;
	QAction *downloadAction;
	QAction *queueAction;
	QAction *sendToAction;
	QAction *editAction;
	QAction *readAction;
	QAction *changePermissionAction;
	QAction *delAction;
	QAction *renameAction;
	QAction *propertyAction;
	TinyFTP *parentTinyFtp;
};

#endif // REMOTEDIRWIDGET_H
