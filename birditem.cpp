#include "birditem.h"
#include <QTimer>
#include <QGraphicsScene>

BirdItem::BirdItem(QPixmap pixmap) :
    wingState(WingState::Up),
    wingDirection(false)    // 默认翅膀方向朝下
{
    setPixmap(pixmap);
    QTimer *birdWingsTimer = new QTimer(this);  // 使用计时器定时改变小鸟翅膀的状态
    connect(birdWingsTimer, &QTimer::timeout, [=](){
        updatePixmap();
    });
    birdWingsTimer->start(80);  // 拍打间隔时间（ms）

    groundPosition = scenePos().y() + 230;
    topPosition = scenePos().y() - 260;

    yAnimation = new QPropertyAnimation(this, "y", this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

//    yAnimation->start();

    rotationAnimation = new QPropertyAnimation(this, "rotation", this);

//    rotateTo(90, 1200, QEasingCurve::InQuad);
}

void BirdItem::updatePixmap()
{
    if ( wingState == WingState::Middle )
    {
        if ( wingDirection )    // 翅膀方向朝上
        {
            setPixmap(QPixmap(":/images/bird_blue_up.png"));
            wingState = WingState::Up;
            wingDirection = false;
        }
        else    // 翅膀方向朝下
        {
            setPixmap(QPixmap(":/images/bird_blue_down.png"));
            wingState = WingState::Down;
            wingDirection = true;
        }
    }
    else
    {
        setPixmap(QPixmap(":/images/bird_blue_middle.png"));
        wingState = WingState::Middle;
    }
}

bool BirdItem::birdOnTheGroundOrTop()
{
    if ( y() >= groundPosition || y() <= topPosition )
    {
        return true;
    }
    return false;
}

qreal BirdItem::y() const
{
    return m_y;
}

void BirdItem::setY(qreal newY)
{
//    qDebug() << "new_Y = " << newY << "m_y = " << m_y << "newY - m_y = " << newY - m_y;
    qreal dy = newY - m_y;
    moveBy(0, dy);  //moveBy(dx, dy);
    m_y = newY;
    if ( birdOnTheGroundOrTop() )
    {
        emit gameFail();
    }
}

void BirdItem::setRotation(qreal newRotation)
{
    m_rotation = newRotation;

    QPointF center = boundingRect().center();

    QTransform t;
    t.translate(center.x(), center.y());
    t.rotate(newRotation);
    t.translate(-center.x(), -center.y());
    setTransform(t);
}

void BirdItem::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPos_y = y();

    yAnimation->setStartValue(curPos_y);
    yAnimation->setEndValue(curPos_y - scene()->sceneRect().height() / 8);  //小鸟弹跳的高度
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(285);   // 弹起所花费时间

    connect(yAnimation, &QPropertyAnimation::finished, [=](){
        fallDown();
    });

    yAnimation->start();

    // 将小鸟旋转回来
    rotateTo(-20, 200, QEasingCurve::OutCubic);
}

void BirdItem::startFly()
{
    yAnimation->start();
    rotateTo(90, 1200, QEasingCurve::InQuad);
}

void BirdItem::freezeBird()
{
    yAnimation->stop();
    rotationAnimation->stop();
}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);
    rotationAnimation->start();
}

void BirdItem::fallDown()
{
    if ( y() < groundPosition )
    {
        rotationAnimation->stop();
        yAnimation->setStartValue(y());
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setEndValue(groundPosition);
        yAnimation->setDuration(800);   // 下落时间
        yAnimation->start();

        rotateTo(90, 1200, QEasingCurve::InCubic);
    }
}

qreal BirdItem::rotation() const
{
    return m_rotation;
}
