#ifndef TINYFTP_H
#define TINYFTP_H

#include <QWidget>
#include <QtGui>
#include <QFtp>
#include "dirtablemodel.h"
#include "localdirwidget.h"
#include "remotedirwidget.h"

class TinyFTP : public QMainWindow
{
	Q_OBJECT

public:
	TinyFTP(QWidget *parent = 0);
	~TinyFTP();
protected:
	void writeSettings();
	void readSettings();
	void closeEvent(QCloseEvent *event);
private:
	QLabel *userNameLabel;
	QLineEdit *userNameLineEdit;
	QLabel *passwordLabel;
	QLineEdit *passwordLineEdit;
	QLabel *portLabel;
	QLineEdit *portLineEdit;
	QCheckBox *anonymousCheckBox;
	QLabel *addressLabel;
	QComboBox *addressComboBox;
	QPushButton *goPushButton;

	QTabWidget *localDirTabWidget;
	QTabWidget *remoteDirTabWidget;

	QSplitter *splitter;
	QStatusBar *ftpStatusBar;
	QFtp *ftp;
};

#endif // TINYFTP_H
