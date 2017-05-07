#ifndef REMOTEDIRWIDGET_H
#define REMOTEDIRWIDGET_H

#include <QtGui>
#include <QFtp>
#include "dirtreemodel.h"
#include "ftpclient.h"

class RemoteDirWidget : public QWidget
{
	Q_OBJECT

public:
	RemoteDirWidget(QWidget *parent = 0);
	~RemoteDirWidget();
    void writeLog(const QString &logData);
//     void setLoginInfo(const QString &port, const QString &address, 
//         const QString &usrname = QString(), const QString &pwd = QString());
    bool getDirectory(const QString &address, const QString &port, const QString &usrname = QString(), 
        const QString &pwd = QString());
    public slots:
        void ftpListInfo(const QUrlInfo &urlInfo);
        void itemDoubleClicked(QTreeWidgetItem *item);
        void ftpDone(bool error);
signals:
    void updateLoginInfo(const QString &usrname, 
        const QString &pwd, const QString &port, 
        const QString &address, bool isanonymous);
private:
    void processDirectory(const QString &dir);
	DirTreeModel *remoteDirTreeModel;
	QTreeView *remoteDirTreeView;
	QFileSystemModel *remoteDirFileSystemModel;
	QComboBox *remoteDirComboBox;
	QToolButton *dotdotDirToolButton;
	QToolButton *refreshDirToolButton;
	QStatusBar *remoteDirStatusBar;
	QTextEdit *logTextEdit;
//     QString username;
//     QString password;
//     QString port;
//     QString address;
    FTPClient *ftpClient;
    QFileIconProvider provider;
    QStringList pendingDirs;
    QString currentDir;
    QString currentLocalDir;
};

#endif // REMOTEDIRWIDGET_H
