#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

Scene::Scene(QObject *parent) : QGraphicsScene(parent),
    gameOn(false),
    score(0),
    bestScore(0)
{
    setUpPipeTimer();   // 定时生成柱子和检测碰撞
    setUpBirdTimer();   // 定时检测小鸟的y坐标
}

void Scene::addBird()
{
    // 初始化小鸟
    bird = new BirdItem(QPixmap(":/images/bird_blue_up.png"));
    bird->setPos(0, 0);
    addItem(bird);
}

void Scene::startGame()
{
    // 停止计时器，无论它们处于什么状态
    PipeTimer->stop();
    BirdTimer->stop();

    hideGameOverGraphics();

    // 使小鸟开始飞行前检查bird是否是空指针
    if( bird != nullptr )
    {
        bird->startFly();
    }

    // 清除和开始创建新的柱子
    if ( !PipeTimer->isActive() )
    {
        cleanPipe();
        setScore(0);
        setGameOn(true);
        PipeTimer->start(1000); // 开启计时器
    }

    if ( !BirdTimer->isActive() )
    {
        cleanPipe();
        setScore(0);
        setGameOn(true);
        BirdTimer->start(1000); // 开启计时器
    }
}

void Scene::setUpPipeTimer()
{
    PipeTimer = new QTimer(this);

    // 当检测到超时信号时创建并添加新的柱子对象到Scene对象中
    connect(PipeTimer, &QTimer::timeout, [=](){
        PipeItem *PipeItem = new class PipeItem();
        connect(PipeItem, &PipeItem::gameFail, [=](){
            stopGame();
        });
        addItem(PipeItem);
    });
}

void Scene::setUpBirdTimer()
{
    BirdTimer = new QTimer(this);
    connect(BirdTimer, &QTimer::timeout, [=](){
        connect(bird, &BirdItem::gameFail, [=](){
            stopGame();
        });
    });
}

void Scene::freezeBirdAndPipe()
{
    // 冻结小鸟
    bird->freezeBird();

    // 冻结柱子
    QList<QGraphicsItem *> sceneItems = items();
    foreach (QGraphicsItem *Item, sceneItems) { // 遍历场景中的所有项目
        PipeItem *pipe = dynamic_cast<PipeItem *>(Item);
        if ( pipe )
        {
            pipe->freezePipe();
        }
    }
}

void Scene::cleanPipe()
{
    QList<QGraphicsItem *> sceneItems = items();
    foreach (QGraphicsItem *Item, sceneItems) { // 遍历场景中的所有项目
        PipeItem *pipe = dynamic_cast<PipeItem *>(Item);
        if ( pipe && pipe->scene() == this )    // 判断pipe是否是空指针和确保pipe是属于该场景的一部分
        {
            // 清除柱子
            removeItem(pipe);
            delete pipe;
            pipe = nullptr;
        }
    }
}

void Scene::showGameOverGraphics()
{
    // 游戏结束提示
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/gameOver.png")); // 游戏结束图像
    addItem(gameOverPix);
    gameOverPix->setPos(QPointF(0, 0) - QPointF(gameOverPix->boundingRect().width() / 2,
                                                gameOverPix->boundingRect().height() / 2));

    // 分数板
    scoreTextItem = new QGraphicsTextItem();
    QString htmlString = "<p> 得分：" + QString::number(score) + "</P>"
            + "<p> 最高得分：" + QString::number(bestScore) + "</p>";
    QFont mFont("Consolas", 30, QFont::Bold);   // 分数展示文字相关设置
    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(mFont);
    scoreTextItem->setDefaultTextColor(Qt::red);
    addItem(scoreTextItem);
    // 与游戏结束提示避让
    scoreTextItem->setPos(QPointF(0, 0) - QPointF(scoreTextItem->boundingRect().width() / 2,
                                                  -gameOverPix->boundingRect().height() / 2));
}

void Scene::hideGameOverGraphics()
{
    if ( gameOverPix )
    {
        removeItem(gameOverPix);
        // -------优秀C++程序员的好习惯-------
        delete gameOverPix; // 防止内存泄漏
        gameOverPix = nullptr;  // 避免野指针
        // --------------------------------
    }
    if ( scoreTextItem )
    {
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

void Scene::stopGame()
{
    hideGameOverGraphics();
    BirdTimer->stop();
    PipeTimer->stop();
    freezeBirdAndPipe();
    setGameOn(false);
    showGameOverGraphics();

    emit gameOver();
}

int Scene::getScore() const
{
    return score;
}

void Scene::setScore(int newScore)
{
    if (score == newScore)
        return;
    score = newScore;
    emit scoreChanged();
}

void Scene::resetScore()
{
    setScore({}); // TODO: Adapt to use your actual default value
}

void Scene::removeBird()
{
    if ( bird && bird->scene() == this )    // 判断bird是否是空指针和确保bird是属于该场景的一部分
    {
        removeItem(bird);
        delete bird;
        bird = nullptr;
    }
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool newGameOn)
{
    gameOn = newGameOn;
}

void Scene::setBestScore(int score)
{
    bestScore = score;
}

int Scene::getBestScore()
{
    return bestScore;
}

void Scene::incrementScore()
{
    score++;
    if ( score > bestScore )
        bestScore = score;

    // 调试语句
//    qDebug() << "当前得分：" << score << "  最高得分：" << bestScore;
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Space )
    {
        if ( gameOn )
        {
            bird->shootUp();
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        if ( gameOn )
        {
            bird->shootUp();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}
