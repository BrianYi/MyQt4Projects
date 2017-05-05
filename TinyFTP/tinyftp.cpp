#include "tinyftp.h"

TinyFTP::TinyFTP(QWidget *parent)
	: QMainWindow(parent)
{
	splitter = new QSplitter(Qt::Horizontal, this);
	
	userNameLabel = new QLabel(tr("�û�:"), this);
	userNameLineEdit = new QLineEdit(this);

	passwordLabel = new QLabel(tr("����:"), this);
	passwordLineEdit = new QLineEdit(this);

	portLabel = new QLabel(tr("�˿�:"), this);
	portLineEdit = new QLineEdit(tr("21"), this);

	anonymousCheckBox = new QCheckBox(tr("����"), this);

	addressLabel = new QLabel(tr("��ַ:"), this);

	addressComboBox = new QComboBox(this);
	addressComboBox->setEditable(true);

	goPushButton = new QPushButton(tr("ת��"), this);

	QToolBar *userInfoToolBar = addToolBar(tr("�û���Ϣ"));
	userInfoToolBar->addWidget(userNameLabel);
	userInfoToolBar->addWidget(userNameLineEdit);
	userInfoToolBar->addWidget(passwordLabel);
	userInfoToolBar->addWidget(passwordLineEdit);
	userInfoToolBar->addWidget(portLabel);
	userInfoToolBar->addWidget(portLineEdit);
	userInfoToolBar->addWidget(anonymousCheckBox);
	userInfoToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBarBreak();

	QToolBar *addressInfoToolBar = addToolBar(tr("��ַ��Ϣ"));
	addressInfoToolBar->addWidget(addressLabel);
	addressInfoToolBar->addWidget(addressComboBox);
	addressInfoToolBar->addWidget(goPushButton);
	addressInfoToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

	localDirTabWidget = new QTabWidget(this);
	localDirTabWidget->addTab(new LocalDirWidget(this), tr("����"));

	remoteDirTabWidget = new QTabWidget(this);
	remoteDirTabWidget->addTab(new RemoteDirWidget(this), tr(" "));

	ftpStatusBar = statusBar();

	ftp = new QFtp(this);

	splitter->addWidget(localDirTabWidget);
	splitter->addWidget(remoteDirTabWidget);
	splitter->setStretchFactor(1, 1);
	setCentralWidget(splitter);

	readSettings();

	setWindowTitle(tr("TinyFTP"));
}

TinyFTP::~TinyFTP()
{
}

void TinyFTP::writeSettings()
{
	QSettings settings(tr("MyQt4Projects"), tr("TinyFTP"));
	settings.setValue("geometry", saveGeometry());
}

void TinyFTP::readSettings()
{
	QSettings settings(tr("MyQt4Projects"), tr("TinyFTP"));
	restoreState(settings.value("geometry").toByteArray());
}

void TinyFTP::closeEvent(QCloseEvent *event)
{
	writeSettings();
}
