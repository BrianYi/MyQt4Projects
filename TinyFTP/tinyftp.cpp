#include "tinyftp.h"

TinyFTP::TinyFTP(QWidget *parent)
	: QMainWindow(parent)
{
	splitter = new QSplitter(Qt::Horizontal, this);
	
	userNameLabel = new QLabel(tr("用户:"), this);
	userNameLineEdit = new QLineEdit(this);

	passwordLabel = new QLabel(tr("口令:"), this);
	passwordLineEdit = new QLineEdit(this);

	portLabel = new QLabel(tr("端口:"), this);
	portLineEdit = new QLineEdit(tr("21"), this);

	anonymousCheckBox = new QCheckBox(tr("匿名"), this);

	addressLabel = new QLabel(tr("地址:"), this);

	addressComboBox = new QComboBox(this);
	addressComboBox->setEditable(true);

	goPushButton = new QPushButton(tr("转到"), this);

	QToolBar *userInfoToolBar = addToolBar(tr("用户信息"));
	userInfoToolBar->addWidget(userNameLabel);
	userInfoToolBar->addWidget(userNameLineEdit);
	userInfoToolBar->addWidget(passwordLabel);
	userInfoToolBar->addWidget(passwordLineEdit);
	userInfoToolBar->addWidget(portLabel);
	userInfoToolBar->addWidget(portLineEdit);
	userInfoToolBar->addWidget(anonymousCheckBox);
	userInfoToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBarBreak();

	QToolBar *addressInfoToolBar = addToolBar(tr("地址信息"));
	addressInfoToolBar->addWidget(addressLabel);
	addressInfoToolBar->addWidget(addressComboBox);
	addressInfoToolBar->addWidget(goPushButton);
	addressInfoToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

	localDirTabWidget = new QTabWidget(this);
	localDirTabWidget->addTab(new LocalDirWidget(this), tr("本地"));

	remoteDirTabWidget = new QTabWidget(this);
	remoteDirTabWidget->addTab(new RemoteDirWidget(this), tr(" "));

	ftpStatusBar = statusBar();

	splitter->addWidget(localDirTabWidget);
	splitter->addWidget(remoteDirTabWidget);
//     splitter->setStretchFactor(0, 1);
// 	splitter->setStretchFactor(1, 1);
	setCentralWidget(splitter);

	readSettings();

    addressComboBox->addItems(addressList);

	setWindowTitle(tr("TinyFTP"));

    connect(goPushButton, SIGNAL(clicked()), this, SLOT(connectToFTPServer()));
    connect(anonymousCheckBox, SIGNAL(stateChanged(int)), this, SLOT(anonymous(int)));
}

void TinyFTP::writeSettings()
{
	QSettings settings(tr("MyQt4Projects"), tr("TinyFTP"));
    //settings.beginGroup("TinyFTP");
	settings.setValue("geometry", saveGeometry());
    settings.setValue("addressList", addressList);
//     settings.endGroup();
//     settings.beginGroup("localDirTabWidget");
//     settings.setValue("geometry", localDirTabWidget->saveGeometry());
//     settings.endGroup();
//     settings.beginGroup("remoteDirTabWidget");
//     settings.setValue("geometry", remoteDirTabWidget->saveGeometry());
//     settings.endGroup();
}

void TinyFTP::readSettings()
{
	QSettings settings(tr("MyQt4Projects"), tr("TinyFTP"));
    //settings.beginGroup("TinyFTP");
    restoreGeometry(settings.value("geometry").toByteArray());
    addressList = settings.value("addressList").toStringList();
//     settings.endGroup();
//     settings.beginGroup("localDirTabWidget");
//     localDirTabWidget->restoreGeometry(settings.value("geometry").toByteArray());
//     settings.endGroup();
//     settings.beginGroup("remoteDirTabWidget");
//     remoteDirTabWidget->restoreGeometry(settings.value("geometry").toByteArray());
//     settings.endGroup();
}

void TinyFTP::closeEvent(QCloseEvent *event)
{
	writeSettings();
}

void TinyFTP::connectToFTPServer()
{
    if (!okToConnectToFTPServer()) {
        return ;
    }

    QString port = portLineEdit->text();
    QString address = addressComboBox->currentText();
    QString username = userNameLineEdit->text();
    QString password = passwordLineEdit->text();
    if (addressComboBox->findText(address) == -1) {
        addressComboBox->addItem(address);
        addressList.append(address);
    }
    RemoteDirWidget *remoteDirWidget = qobject_cast<RemoteDirWidget*>(remoteDirTabWidget->currentWidget());
    if (anonymousCheckBox->isChecked()) {
        username = tr("");
        password = tr("");
    }
    remoteDirTabWidget->setTabText(remoteDirTabWidget->currentIndex(), address);
    remoteDirWidget->getDirectory(address, port, address, username, password);
}

bool TinyFTP::okToConnectToFTPServer()
{
    if (!anonymousCheckBox->isChecked()) {
        if (userNameLineEdit->text().isEmpty() ||
            passwordLineEdit->text().isEmpty()) {
                return false;
        }
    }

    if (portLineEdit->text().isEmpty() || 
        addressComboBox->currentText().isEmpty())
        return false;

    return true;
}

void TinyFTP::anonymous(int state)
{
    if (state == Qt::Checked) {
        userNameLineEdit->setEnabled(false);
        passwordLineEdit->setEnabled(false);
    } else {
        userNameLineEdit->setEnabled(true);
        passwordLineEdit->setEnabled(true);
    }
}
