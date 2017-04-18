#ifndef MAILCLIENT_H
#define MAILCLIENT_H

#include <QtGui>

class mailclient : public QMainWindow
{
    Q_OBJECT

public:
    mailclient(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~mailclient();
protected:
    void closeEvent(QCloseEvent *event);
private:
    void addFolder(const QIcon &icon, const QString &name);
    void addMessage(const QString &subject, const QString &from, const QString &date);
    void readSettings();
    void writeSettings();

    QSplitter *mainSplitter;
    QSplitter *rightSplitter;
    QTreeWidget *foldersTreeWidget;
    QTreeWidget *messagesTreeWidget;
    QTextEdit *textEdit;
};

#endif // MAILCLIENT_H
