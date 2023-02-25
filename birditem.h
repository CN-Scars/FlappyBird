#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class BirdItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    explicit BirdItem(QPixmap pixmap);
    qreal y() const;
    void setY(qreal newY);

    qreal rotation() const;
    void setRotation(qreal newRotation);

    void shootUp(); //实现让小鸟通过点击从而上升的方法
    void startFly();
    void freezeBird();

public slots:
    void rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve);    //旋转小鸟的方法

    void fallDown();

private:
    enum WingState {    //存储翅膀状态的枚举型
        Up,
        Middle,
        Down
    };
    void updatePixmap();    //更改小鸟翅膀状态

    WingState wingState;
    bool wingDirection; //true：翅膀方向朝上，false：翅膀方向朝下
    qreal m_y;
    qreal m_rotation;
    QPropertyAnimation *yAnimation;
    QPropertyAnimation *rotationAnimation;  //旋转动画
    qreal groundPosition;   //地面的位置
    qreal topPosition;  //顶部的位置

    bool birdOnTheGroundOrTop();

signals:
    void gameFail(); //游戏失败信号

};

#endif // BIRDITEM_H
