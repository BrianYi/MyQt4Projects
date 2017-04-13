#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "spreadsheet.h"
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
class FindDialog;
class GoToCellDialog;
class SortDialog;
class Spreadsheet;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void find();
	void gotocell();
	void sort();
	void about();
	void openRecentFile();
	void updateStatusBar();
	void spreadsheetModified();
protected:
	void createActions();
	void createMenus();
	void createContextMenu();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	void updateRecentFileActions();
	bool okToContinue();
	void closeEvent(QCloseEvent *event);
	bool loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
private:
	Spreadsheet *spreadsheet;
    FindDialog *findDialog;
    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction;

	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *delAction;
	QAction *selAction;
	QAction *rowAction;
	QAction *columnAction;
	QAction *allAction;

	QAction *findAction;
	QAction *gotocellAction;

	QAction *recalculateAction;
	QAction *sortAction;

	QAction *showGridAction;
	QAction *autoRecalculateAction;

	QAction *aboutAction;
	QAction *aboutQtAction;
	QStringList recentFiles;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QLabel *locationLabel;
	QLabel *formulaLabel;
	QString curFile;
};

#endif // MAINWINDOW_H
