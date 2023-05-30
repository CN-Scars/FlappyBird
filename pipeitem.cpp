#include "scene.h"
#include "pipeitem.h"
#include "birditem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QDebug>

PipeItem::PipeItem() :
    topPipe(new QGraphicsPixmapItem(QPixmap(":/images/pipe_down.png"))),
    bottomPipe(new QGraphicsPixmapItem(QPixmap(":/images/pipe_up.png"))),
    birdPassed(false)
{
    topPipe->setPos(QPointF(0, 0) - QPointF(topPipe->boundingRect().width() / 2,    // x轴坐标
                    topPipe->boundingRect().height() + 60));    // y轴坐标，增加距离以创建空隙
    bottomPipe->setPos(QPointF(0, 0) + QPointF(-bottomPipe->boundingRect().width() / 2,
                                               60));

    // 把上、下两个柱子加入到一个组内形成柱子组
    addToGroup(topPipe);
    addToGroup(bottomPipe);

    // -------------------------随机生成x、y的值----------------------------------
    int xRandom = QRandomGenerator::global()->bounded(200); //随机生成柱子x坐标
    yRandom = QRandomGenerator::global()->bounded(150);    //随机生成柱子y坐标
    setPos(QPoint(0, 0) + QPoint(260 + xRandom, yRandom));  //设置柱子组的出现位置
    // -------------------------------------------------------------------------

    // 创建动画对象
    xAnimation = new QPropertyAnimation(this, "x", this);

    // --设置柱子组的可见范围以实现柱子组的左右移动--
    xAnimation->setStartValue(260 + xRandom);
    xAnimation->setEndValue(-260);
    // ---------------------------------------
    xAnimation->setEasingCurve(QEasingCurve::Linear);   // 设置缓和曲线
    xAnimation->setDuration(2500);  // 设置动画持续时间

    // 当动画结束时从内存释放PipeItem对象
    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        scene()->removeItem(this);
        delete this;    // 从内存中删除对象
    });
    xAnimation->start();
}

PipeItem::~PipeItem()
{
//    qDebug() << "柱子组已被释放";
    // 防止内存泄漏
    delete topPipe;
    delete bottomPipe;
}

qreal PipeItem::x() const
{
    return m_x;
}

void PipeItem::setX(qreal newX)
{
    m_x = newX;

    if ( newX < 0 && !birdPassed )  // 柱子还未加载且未有小鸟穿过
    {
        birdPassed = true;
        QGraphicsScene *mScene = scene();
        Scene *myScene = dynamic_cast<Scene *>(mScene);
        if ( myScene )
        {
            myScene->incrementScore();
        }
    }

    if ( collideWithBird() )
    {
        emit gameFail();
    }
    setPos(QPointF(0, 0) + QPointF(newX, yRandom));
}

void PipeItem::freezePipe()
{
    xAnimation->stop();
}

bool PipeItem::collideWithBird()
{
    QList<QGraphicsItem*> collidingItems = topPipe->collidingItems();
    collidingItems.append(bottomPipe->collidingItems());
    foreach (QGraphicsItem *Item, collidingItems) {
        BirdItem *birdItem = dynamic_cast<BirdItem*>(Item);
        if ( birdItem )
        {
//            qDebug() << "碰到柱子";
            return true;
        }
    }
    return false;
}
