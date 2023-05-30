#ifndef SCENE_H
#define SCENE_H

#include "birditem.h"
#include "pipeitem.h"
#include <QGraphicsScene>
#include <QTimer>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();

    void startGame();   // 开始游戏的方法

    bool getGameOn() const;
    void setGameOn(bool newGameOn);

    void setBestScore(int score);
    int getBestScore();
    void incrementScore();  // 加分方法

    int getScore() const;
    void setScore(int newScore);
    void resetScore();

    void removeBird();

public slots:
    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event) override;  // 空格敲击弹起
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // 鼠标左键敲击弹起

private:
    //放入PipeItem类的私有方法
    void setUpPipeTimer();  // 使用计时器定时生成柱子
    void setUpBirdTimer();
    void freezeBirdAndPipe();   // 游戏结束后固定小鸟和柱子

    void cleanPipe();   // 游戏失败后清除柱子的方法

    void showGameOverGraphics();
    void hideGameOverGraphics();

    void stopGame();

    QTimer *PipeTimer;
    QTimer *BirdTimer;
    BirdItem *bird;
    bool gameOn;

    // 存储分数
    int score;
    int bestScore;

    // 展示游戏结束相关信息
    QGraphicsPixmapItem *gameOverPix;
    QGraphicsTextItem *scoreTextItem;

    Q_PROPERTY(int score READ getScore WRITE setScore RESET resetScore NOTIFY scoreChanged)

signals:

    void scoreChanged();
    void gameOver();
};

#endif // SCENE_H
