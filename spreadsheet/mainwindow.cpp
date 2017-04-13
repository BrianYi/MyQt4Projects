#include "mainwindow.h"


MainWindow::MainWindow()
{
	spreadsheet = new Spreadsheet;
	setCentralWidget(spreadsheet);

	createActions();
	createMenus();
	createContextMenu();
	createToolBars();
	createStatusBar();
	readSettings();

    findDialog = 0;
	
	setWindowIcon(QIcon(":/images/icon.png"));
	setCurrentFile("");
}

void MainWindow::createActions()
{
	/* File */
	newAction = new QAction(tr("&New"), this);
	newAction->setIcon(QIcon(":/images/new.png"));
	newAction->setShortcut(tr("Ctrl+N"));
	newAction->setStatusTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openAction = new QAction(tr("&Open..."), this);
	openAction->setIcon(QIcon(":/images/open.png"));
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setStatusTip(tr("Open a file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setIcon(QIcon(":/images/save.png"));
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("Save a file"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAction = new QAction(tr("Save&As..."), this);
	saveAsAction->setIcon(QIcon(":/images/save.png"));
	saveAsAction->setStatusTip(tr("Save a file as..."));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; i++) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

	exitAction = new QAction(tr("E&xit..."), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	/* Edit */
	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setIcon(QIcon(":/images/cut.png"));
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setIcon(QIcon(":/images/copy.png"));
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setIcon(QIcon(":/images/paste.png"));
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));

	delAction = new QAction(tr("&Delete"), this);
	delAction->setShortcut(tr("Del"));
	delAction->setStatusTip(tr("Delete"));
    connect(delAction, SIGNAL(triggered()), spreadsheet, SLOT(del()));


	rowAction = new QAction(tr("&Row"), this);
	rowAction->setStatusTip(tr("Row"));
    connect(rowAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentRow()));

	columnAction = new QAction(tr("&Column"), this);
	columnAction->setStatusTip(tr("Column"));
    connect(columnAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentColumn()));

	allAction = new QAction(tr("&All"), this);
	allAction->setShortcut(tr("Ctrl+A"));
	allAction->setStatusTip(tr("Select all"));
    connect(allAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));

	findAction = new QAction(tr("&Find..."), this);
	findAction->setIcon(QIcon(":/images/find.png"));
	findAction->setShortcut(tr("Ctrl+F"));
	findAction->setStatusTip(tr("Find"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

	gotocellAction = new QAction(tr("&Go to Cell..."), this);
	gotocellAction->setIcon(QIcon(":/images/gotocell.png"));
	gotocellAction->setShortcut(tr("Ctrl+G"));
	gotocellAction->setStatusTip(tr("Go to cell..."));
    connect(gotocellAction, SIGNAL(triggered()), this, SLOT(gotocell()));

	/* Tools */
	recalculateAction = new QAction(tr("&Recalculate"), this);
	recalculateAction->setShortcut(tr("F9"));
	recalculateAction->setStatusTip(tr("Recalculate"));
    connect(recalculateAction, SIGNAL(triggered()), spreadsheet, SLOT(recalculate()));

	sortAction = new QAction(tr("&Sort..."), this);
	sortAction->setStatusTip(tr("Sort"));
    connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));

	/* Options */
	showGridAction = new QAction(tr("&Show Grid"), this);
	showGridAction->setCheckable(true);
	showGridAction->setChecked(true);
	showGridAction->setStatusTip(tr("Show grid"));
    connect(showGridAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setShowGrid(bool)));

	autoRecalculateAction = new QAction(tr("&Auto-Recalculate"), this);
	autoRecalculateAction->setCheckable(true);
	autoRecalculateAction->setChecked(true);
	autoRecalculateAction->setStatusTip(tr("Auto-Recalculate"));
    connect(autoRecalculateAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setAutoRecalculate(bool)));

	/* Help */
	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("About this software"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("About Qt info."));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; i++) {
        fileMenu->addAction(recentFileActions[i]);
    }
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(delAction);
	QMenu *selMenu = editMenu->addMenu(tr("&Select"));
	selMenu->addAction(rowAction);
	selMenu->addAction(columnAction);
	selMenu->addAction(allAction);
	editMenu->addMenu(selMenu);
	editMenu->addSeparator();
	editMenu->addAction(findAction);
	editMenu->addAction(gotocellAction);

	QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(recalculateAction);
	toolsMenu->addAction(sortAction);

	QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
	optionsMenu->addAction(showGridAction);
	optionsMenu->addAction(autoRecalculateAction);

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu()
{
	spreadsheet->addAction(cutAction);
	spreadsheet->addAction(copyAction);
	spreadsheet->addAction(pasteAction);
	spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);
	editToolBar->addSeparator();
	editToolBar->addAction(findAction);
	editToolBar->addAction(gotocellAction);
}

void MainWindow::createStatusBar()
{
	locationLabel = new QLabel(tr(" W999 "));
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());

	formulaLabel = new QLabel;
	formulaLabel->setIndent(3);

	statusBar()->addWidget(locationLabel);
	statusBar()->addWidget(formulaLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()), this, SLOT(spreadsheetModified()));
}

void MainWindow::readSettings()
{
	QSettings settings("MyQt4Projects", "Spreadsheet");

	restoreGeometry(settings.value("geometry").toByteArray());

	recentFiles = settings.value("recentFiles").toStringList();
	updateRecentFileActions();

	bool showGrid = settings.value("showGrid", true).toBool();
	showGridAction->setChecked(showGrid);

	bool autoRecalculate = settings.value("autoRecalculate", true).toBool();
	autoRecalculateAction->setChecked(autoRecalculate);
}

void MainWindow::writeSettings()
{
	QSettings settings("MyQt4Projects", "Spreadsheet");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("recentFiles", recentFiles);
	settings.setValue("showGrid", showGridAction->isChecked());
	settings.setValue("autoRecalculate", autoRecalculateAction->isChecked());
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next())) {
            i.remove();
        }
    }

    for (int j = 0; j < MaxRecentFiles; j++) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (okToContinue())
	{
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

bool MainWindow::okToContinue()
{
	if (isWindowModified()) {
		int r = QMessageBox::warning(this, tr("Spreadsheet"), tr("The document has been modified.\n"
			"Do you want to save your changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
		{
			return save();
		} else if (r == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	setWindowModified(false);

	QString shownName = tr("Untitled");
	if (!curFile.isEmpty())
	{
		shownName = strippedName(curFile);
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("MySpreadsheet")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::newFile()
{
	if (okToContinue())
	{
		spreadsheet->clear();
		setCurrentFile("");
	}
}

void MainWindow::open()
{
	if (okToContinue())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spreadsheet"), ".", tr("Spreadsheet file (*.sp)"));

		if (!fileName.isEmpty())
		{
			loadFile(fileName);
		}
	}
}

bool MainWindow::save()
{
	if (curFile.isEmpty())
	{
		return saveAs();
	}
	else
	{
		return saveFile(curFile);
	}
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"), ".", tr("Spreadsheet files (*.sp)"));
	if (fileName.isEmpty()) {
		return false;
	}

	return saveFile(fileName);
}

void MainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), spreadsheet, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)), spreadsheet, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::gotocell()
{
    GoToCellDialog dialog(this);
    if (dialog.exec()) {
        QString str = dialog.ui.lineEdit->text().toUpper();
        spreadsheet->setCurrentCell(str.mid(1).toInt() - 1, str[0].unicode() - 'A');
    }
}

void MainWindow::sort()
{
    SortDialog dialog(this);
    QTableWidgetSelectionRange range = spreadsheet->selectedRange();
    dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());

    if (dialog.exec()) {
        SpreadsheetCompare compare;
        compare.keys[0] = dialog.ui.primaryColumnCombo->currentIndex();
        compare.keys[1] = dialog.ui.secondaryColumnCombo->currentIndex() - 1;
        compare.keys[2] = dialog.ui.tertiaryColumnCombo->currentIndex() - 1;
        compare.ascending[0] = (dialog.ui.primaryOrderCombo->currentIndex() == 0);
        compare.ascending[1] = (dialog.ui.secondaryOrderCombo->currentIndex() == 0);
        compare.ascending[2] = (dialog.ui.tertiaryOrderCombo->currentIndex() == 0);
        spreadsheet->sort(compare);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Spreadsheet"),
        tr("<h2>Spreadsheet 1.1</h2>"
        "<p>Copyright &copy; 2008 Software Inc."
        "<p>Spreadsheet is a small application that "
        "demonstrates QAction, QMainWindow, QMenuBar, "
        "QStatusBar, QTableWidget, QToolBar, and many other "
        "Qt classes."));
}

void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

bool MainWindow::loadFile(const QString &fileName)
{
	if (!spreadsheet->readFile(fileName)) {
		statusBar()->showMessage(tr("Loading canceled"), 2000);
		return false;
	}

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
	return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
	if (!spreadsheet->writeFile(fileName)) {
		statusBar()->showMessage(tr("Saving canceled"), 2000);
		return false;
	}

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

