#include "snakegame.h"
#include <time.h>
#include "QPoint"

SnakeGame::SnakeGame(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setAttribute(Qt::WA_StaticContents);
    zoom = 30;
    imageMap = QImage(9, 9, QImage::Format_ARGB32);
    imageMap.fill(qRgba(0, 0, 0, 0));
    srand(time(0));
    setFixedSize(9 * zoom + 1, 9 * zoom + 1);
}

void SnakeGame::begin(Level lvl)
{
    tickTimer.setInterval(lvl);
    direction = Right;
    head = tail = new SnakeNode;
    head->pos = QPoint(imageMap.width() / 2, imageMap.height() / 2);
    backgroundColor = qRgb(0, 183, 0);
    gridColor = backgroundColor;
    snakeColor = Qt::black;
    foodColor = Qt::yellow;
    imageMap.fill(backgroundColor);
    setPixel(head->pos.x(), head->pos.y(), snakeColor);
    generateFood();
    connect(&tickTimer, SIGNAL(timeout()), this, SLOT(move()));
    connect(this, SIGNAL(gameOver()), this, SLOT(stop()));
    tickTimer.start();
}

void SnakeGame::stop()
{
    //tickTimer.stop();
}

void SnakeGame::setMapSize(int w, int h)
{
    imageMap = QImage(w, h, QImage::Format_ARGB32);
    imageMap.fill(qRgba(0, 0, 0, 0));
    setFixedSize(w * zoom + 1, h * zoom + 1);
    update();
    updateGeometry();
}

void SnakeGame::keyPressEvent(QKeyEvent * event)
{
    switch (event->key())
    {
        case Qt::Key_Up:
            if (direction != Down)
            {
                direction = Up;
            }
            break;
        case Qt::Key_Down:
            if (direction != Up)
            {
                direction = Down;
            }
            break;
        case Qt::Key_Left:
            if (direction != Right)
            {
                direction = Left;
            }
            break;
        case Qt::Key_Right:
            if (direction != Left)
            {
                direction = Right;
            }
            break;
        default:
            break;
    }
    move();
}


void SnakeGame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(gridColor);
    for (int i = 0; i <= imageMap.width(); i++) {
        painter.drawLine(i * zoom, 0, i * zoom, zoom * imageMap.height());
    }
    for (int j = 0; j <= imageMap.height(); j++) {
        painter.drawLine(0, j * zoom, zoom * imageMap.width(), j * zoom);
    }
    painter.drawImage(QRect(0, 0, imageMap.width(), imageMap.height()), QImage(":images/background.jpg"));
    for (int i = 0; i < imageMap.width(); i++) {
        for (int j = 0; j < imageMap.height(); j++) {
            QColor rectColor = QColor::fromRgb(imageMap.pixel(i, j));
            QRect rect(i * zoom + 1, j * zoom + 1, zoom - 1, zoom - 1);
            //painter.fillRect(rect, rectColor);
            painter.fillRect(rect, backgroundColor);
            if (rectColor == backgroundColor)
                painter.fillRect(rect, backgroundColor);
            else if (rectColor == foodColor) {
                 painter.drawImage(rect, QImage(":images/food.png"));
            } else if (rectColor == snakeColor) {
                if (head->pos.x() == i && head->pos.y() == j)
                {
                    if (direction == Right)
                        painter.drawImage(rect, QImage(":images/head-right.png"));
                    else if (direction == Down) {
                        painter.drawImage(rect, QImage(":images/head-down.png"));
                    } else if (direction == Left) {
                        painter.drawImage(rect, QImage(":images/head-left.png"));
                    } else if (direction == Up) {
                        painter.drawImage(rect, QImage(":images/head-up.png"));
                    }
                }
                else
                    painter.drawImage(rect, QImage(":images/body.png"));
            }
           
        }
    }
}

void SnakeGame::move()
{ 
    switch (direction) 
    {
    case Up:
        moveUp();
        break;
    case Down:
        moveDown();
        break;
    case Left:
        moveLeft();
        break;
    case Right:
        moveRight();
        break;
    default:
        break;
    }
    update();
}

void SnakeGame::moveUp()
{
    operate(head->pos.x(), head->pos.y() - 1);
}

void SnakeGame::moveDown()
{
    operate(head->pos.x(), head->pos.y() + 1);
}

void SnakeGame::moveLeft()
{
    operate(head->pos.x() - 1, head->pos.y());
}

void SnakeGame::moveRight()
{
    operate(head->pos.x() + 1, head->pos.y());
}

void SnakeGame::generateFood()
{
    QColor rgb = foodColor;
    QPoint foodPos;
    while (rgb != backgroundColor) {
        foodPos = QPoint(rand() % imageMap.width(), rand() % imageMap.height());
        rgb = QColor::fromRgba(imageMap.pixel(foodPos));
    }
    setPixel(foodPos.x(), foodPos.y(), foodColor);
}

QRect SnakeGame::pixelRect(int x, int y) const
{
    return QRect(x * zoom + 1, y * zoom + 1, zoom - 1, zoom - 1);
}

void SnakeGame::setPixel(int x, int y, const QColor &color)
{
    imageMap.setPixel(x, y, color.rgb());
}

QColor SnakeGame::pixel(int x, int y) const
{
    if (imageMap.valid(x, y))
        return QColor::fromRgb(imageMap.pixel(x, y));
    return QColor();
}

void SnakeGame::operate(int x, int y)
{
    QColor pxColor = pixel(x, y);
    if (!imageMap.valid(x, y) || pxColor == snakeColor)
        emit gameOver();
    if (pxColor == backgroundColor) {
        setPixel(x, y, snakeColor);
        setPixel(tail->pos.x(), tail->pos.y(), backgroundColor);
        SnakeNode *p = new SnakeNode;
        p->pos = QPoint(x, y);
        p->next = head;
        head = p;
        SnakeNode *pre = 0;
        while (p->next) {
            pre = p;
            p = p->next;
        }
        delete tail;
        tail = pre;
        tail->next = 0;
    } else if (pxColor == foodColor) {
        setPixel(x, y, snakeColor);
        SnakeNode *p = new SnakeNode;
        p->pos = QPoint(x, y);
        p->next = head;
        head = p;
        generateFood();
    } 
}


