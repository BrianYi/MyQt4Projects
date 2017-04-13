#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QDialogButtonBox>
#include "ui_gotocelldialog.h"

class GoToCellDialog : public QDialog
{
    Q_OBJECT

public:
    GoToCellDialog(QWidget *parent = 0);

    Ui::GoToCellDialog ui;
private slots:
    void on_lineEdit_textChanged();
};

#endif // GOTOCELLDIALOG_H
