#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	textEdit = new QTextEdit(this);
	setCentralWidget(textEdit);

	textEdit->setAcceptDrops(false);
	this->setAcceptDrops(true);

	setWindowTitle(tr("Text Editor"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list")) {
		event->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty()) {
		return ;
	}

	QString fileName = urls.first().toLocalFile();
	if (fileName.isEmpty()) {
		return ;
	}

	if (readFile(fileName)) {
		setWindowTitle(tr("%1 - %2").arg(fileName).arg(tr("Drag File")));
	}
}

bool MainWindow::readFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("Text Editor"),
			tr("Cannot read file %1:\n%2.")
			.arg(file.fileName())
			.arg(file.errorString()));
		return false;
	}

	QTextStream textStream(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QString txt = textStream.readAll();
	textEdit->setPlainText(txt);
	QApplication::restoreOverrideCursor();
	return true;
}
