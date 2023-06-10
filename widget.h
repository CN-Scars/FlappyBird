#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <scene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event) override;   // 在widget类中重载closeEvent

private slots:
    void on_startGameButton_clicked();

private:
    Ui::Widget *ui;

    int bestscore;

    Scene *scene;   // 创建一个对象返回场景

};

#endif // WIDGET_H
