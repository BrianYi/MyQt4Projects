#ifndef REMOTEDIRWIDGET_H
#define REMOTEDIRWIDGET_H

#include <QtGui>
#include <QFtp>
#include "dirtablemodel.h"
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
    bool getDirectory(QUrl url, const QString &port, const QString &address, 
        const QString &usrname = QString(), const QString &pwd = QString());
    public slots:
        void listInfo(const QUrlInfo &urlInfo);
signals:
    void updateLoginInfo(const QString &usrname, 
        const QString &pwd, const QString &port, 
        const QString &address, bool isanonymous);
private:
	DirTableModel *remoteDirTableModel;
	QTableView *remoteDirTableView;
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
};

#endif // REMOTEDIRWIDGET_H
