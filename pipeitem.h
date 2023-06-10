#ifndef PIPEITEM_H
#define PIPEITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class PipeItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)   // 柱子的属性
public:
    explicit PipeItem();
    ~PipeItem();    // PipeItem对象的析构函数

    qreal x() const;
    void setX(qreal newX);

    void freezePipe();

private:
    bool collideWithBird();
    QGraphicsPixmapItem *topPipe;   // 上方的柱子
    QGraphicsPixmapItem *bottomPipe;    // 下方的柱子
    QPropertyAnimation *xAnimation;
    int yRandom;
    qreal m_x;

    bool birdPassed;

signals:
    void gameFail(); // 游戏失败信号

};

#endif // PIPEITEM_H
