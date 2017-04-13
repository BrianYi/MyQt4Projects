#ifndef HEXSPINBOX_H
#define HEXSPINBOX_H

#include <QSpinBox>

class QRegExpValidator;

class HexSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    HexSpinBox(QWidget *parent);
protected:
    QValidator::State validate(QString &text, int &pos) const;
    int valueFromText(const QString &text) const;
    QString textFromValue(int val) const;

private:
    QRegExpValidator *validator;
    mutable QObject *parent;
};

#endif // HEXSPINBOX_H
