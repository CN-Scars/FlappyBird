#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsPixmapItem>

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
    //创建网格线，便于构建画面
//    scene->addLine(-400, 0, 400, 0, QPen(Qt::blue));
//    scene->addLine(0, -400, 0, 400, QPen(Qt::blue));

    ui->graphicsView->setScene(scene);

    scene->addBird();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startGameButton_clicked()
{
    scene->removeBird();
    scene->addBird();
    scene->startGame();
    ui->startGameButton->setText("重新开始");
}
