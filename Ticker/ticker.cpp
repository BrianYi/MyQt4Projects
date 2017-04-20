#include "ticker.h"

Ticker::Ticker(QWidget *parent)
    : QWidget(parent)
{
    offset = 0;
    myTimerId = 0;
}

Ticker::~Ticker()
{

}

void Ticker::setText(const QString &newText)
{
    myText = newText;
    update();
    updateGeometry();
}

QSize Ticker::sizeHint() const
{
    return fontMetrics().size(0, text());
}

void Ticker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int textWidth = fontMetrics().width(text());
    if (textWidth < 1)
        return ;
    int x = -offset;
    while (x < width()) {
        painter.drawText(x, 0, textWidth, height(), Qt::AlignLeft | Qt::AlignVCenter, myText);
        x += textWidth;
    }
}

void Ticker::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == myTimerId) {
        offset++;
        scroll(-1, 0);
    } else {
        QWidget::timerEvent(event);
    }
}

void Ticker::showEvent(QShowEvent *event)
{
    myTimerId = startTimer(30);
}

void Ticker::hideEvent(QHideEvent *event)
{
    killTimer(myTimerId);
    myTimerId = 0;
}
