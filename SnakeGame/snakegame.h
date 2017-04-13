#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QWidget>
#include <QtGui>

typedef struct _SnakeNode
{
    _SnakeNode() { next = 0; }
    QPoint pos;
    _SnakeNode *next;
}SnakeNode, *PSnakeNode;

class SnakeGame : public QWidget
{
    Q_OBJECT
public:
    enum Direction { Up, Down, Left, Right };
    enum Level { Easy = 1000,  Normal = 500, Hard = 300, Crazy = 100 };
public:
    SnakeGame(QWidget *parent = 0);
    void begin(Level lvl = Easy);
    void setMapSize(int w, int h);
    void setBackgroundColor(const QColor &backgroundColor);
    void setGridColor(const QColor &gridColor);
protected:
    void keyPressEvent ( QKeyEvent * event );
    void paintEvent(QPaintEvent *event);
    public slots:
        void move();
        void stop();
signals:
        void gameOver();
private:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void generateFood();
    QRect pixelRect(int x, int y) const;
    void setPixel(int x, int y, const QColor &color);
    QColor pixel(int x, int y) const;
    void operate(int x, int y);
private:
    int zoom;
    QImage imageMap;
    Direction direction;
    QTimer tickTimer;
    SnakeNode *head;
    SnakeNode *tail;
    QColor backgroundColor;
    QColor gridColor;
    QColor snakeColor;
    QColor foodColor;
};

#endif // SNAKEGAME_H
