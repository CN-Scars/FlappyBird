#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsPixmapItem>
#include <QSettings>
#include <QCloseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new Scene(this);
    scene->setSceneRect(-250, -300, 500, 600);

    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap(":/images/background_day.png"));
    scene->addItem(pixItem);
    pixItem->setPos(QPointF(0, 0) - QPointF(pixItem->boundingRect().width() / 2,
                                            pixItem->boundingRect().height() / 2));
    // 创建网格线，便于构建画面
//    scene->addLine(-400, 0, 400, 0, QPen(Qt::blue));
//    scene->addLine(0, -400, 0, 400, QPen(Qt::blue));

    // 读取最高分
    QSettings settings("Scars", "FlappyBird");
    bestscore = settings.value("bestscore", 0).toInt();
    scene->setBestScore(bestscore);

    ui->graphicsView->setScene(scene);

    scene->addBird();

    connect(scene, &Scene::gameOver, [=] () {
        ui->startGameButton->setEnabled(true);  // 恢复按钮为可点击状态
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event) // 关闭窗口时储存最高分
{
    bestscore = scene->getBestScore();
    QSettings settings("Scars", "FlappyBird");
    settings.setValue("bestscore", bestscore);

    QWidget::closeEvent(event);
}

void Widget::on_startGameButton_clicked()
{
    scene->removeBird();
    scene->addBird();
    scene->startGame();
    ui->startGameButton->setText("重新开始");
    ui->startGameButton->setEnabled(false); // 防止玩家误触重新开始按钮
}
