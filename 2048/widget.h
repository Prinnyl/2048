#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "mypushbutton.h"
#include "myrank.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *);//绘图事件
    void keyPressEvent(QKeyEvent *event);//键盘事件
    bool start;//游戏状态 true正常运行  false停止
    int lattice[6][6];//初始化及保存用数组
    int score = 0;//分数
    void PressUp();//上
    void PressDown();//下
    void PressLeft();//左
    void pressRight();//右
    void forLattice(QPainter *gamePainter, int i, int j, int numB);//格子
    void randEnd();//随机生成

    //剩余空格
    struct Residue
    {
        int i;
        int j;
    };

private:
    Ui::Widget *ui;
    MyPushButton *buttonStart;//开始游戏按钮
    MyPushButton *buttonClose;//退出游戏按钮
    MyPushButton *buttonRank;//退出游戏按钮
    QPainter *gamePainter;
};

#endif // MAINWIDGET_H
