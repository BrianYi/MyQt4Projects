#include "hexspinbox.h"
#include <QMainWindow>
#include <QStatusBar>
HexSpinBox::HexSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    this->parent = parent;
    setRange(0, 255);
    validator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{1,8}"), this);
}

QValidator::State HexSpinBox::validate(QString &text, int &pos) const
{
    return validator->validate(text, pos);
}

int HexSpinBox::valueFromText(const QString &text) const
{
    bool ok;
    return text.toInt(&ok, 16);
}

QString HexSpinBox::textFromValue(int val) const
{
    return QString::number(val, 16).toUpper();
}


