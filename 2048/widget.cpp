#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QKeyEvent>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(400, 700);
    this->setWindowTitle("游戏界面");

    //让上下左右能动,刚进入界面时需点击一下界面
    setFocusPolicy(Qt::StrongFocus);

    //添加数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    //连接数据库
    db.setDatabaseName("prinny");

    //打开数据库
    if(db.open() == false)
    {
        QMessageBox::warning(this, "错误", db.lastError().text());
    }

    //数组初始化
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            lattice[i][j] = 0;
        }
    }
//    lattice[3][4] = 8;
//    lattice[1][2] = 8112;
//    lattice[2][3] = 2048;
//    lattice[3][2] = 256;
//    lattice[2][5] = 512;
//    lattice[4][3] = 1024;
//    lattice[0][0] = 4056;
//    lattice[0][2] = 16224;

    //随机数种子
    qsrand(uint(QTime(0, 0, 0).secsTo(QTime::currentTime())));

    //开始按钮，按完变重新开始
    buttonStart = new MyPushButton(":/new/image/start.png", 0.5);
    buttonStart->setParent(this);
    buttonStart->move(30, 560);
    connect(buttonStart, &MyPushButton::clicked, [=](){
        buttonStart->zoom1();
        buttonStart->zoom2();
        QTimer::singleShot(200, this, [=](){

            //现在时间
            QDateTime curDateTime=QDateTime::currentDateTime();
            //qDebug() << curDateTime;
            //不为0保存数据
            if(score != 0)
            {
                QSqlQuery query;
                query.prepare("insert into scorerank(scoref, timef) values(:score, :time)");
                query.bindValue(":score", score);
                query.bindValue(":time", curDateTime);
                query.exec();
            }

            score = 0;
            for(int i = 0; i < 6; i++)
            {
                for(int j = 0; j < 6; j++)
                {
                    lattice[i][j] = 0;
                }
            }
            buttonStart->setIcon(QPixmap(":/new/image/again.png"));
            int randi = qrand()%4;//0 1 2 3
            int randj = qrand()%4;
            lattice[randi][randj] = 2;
            start = true;
            update();
        });
    });

    //离开游戏
    buttonClose = new MyPushButton(":/new/image/close.png", 0.5);
    buttonClose->setParent(this);
    buttonClose->move(220, 560);
    connect(buttonClose, &MyPushButton::clicked, [=](){
        buttonClose->zoom1();
        buttonClose->zoom2();
        QTimer::singleShot(200, this, [=](){
            QDateTime curDateTime=QDateTime::currentDateTime();
            //qDebug() << curDateTime;

            if(score != 0)
            {
                QSqlQuery query;
                query.prepare("insert into scorerank(scoref, timef) values(:score, :time)");
                query.bindValue(":score", score);
                query.bindValue(":time", curDateTime);
                query.exec();
            }
            this->close();
        });
    });

    //排行榜
    buttonRank = new MyPushButton(":/new/image/rank.png", 1);
    buttonRank->setParent(this);
    buttonRank->move(50, 600);
    connect(buttonRank, &MyPushButton::clicked, [=](){
        buttonRank->zoom1();
        buttonRank->zoom2();
        QTimer::singleShot(200, this, [=](){
            MyRank *rank = new MyRank;
            rank->setGeometry(this->geometry());
            rank->show();
            this->hide();

            connect(rank, &MyRank::signalReturn,[=](){
                this->setGeometry(rank->geometry());
                this->show();
                delete rank;
            });
        });
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    //背景
    QPainter groundPainter(this);
    groundPainter.drawPixmap(0, 0, QPixmap(":/new/image/57526556_p0.png"));

    //分数
    gamePainter = new QPainter(this);
    gamePainter->setFont(QFont("华文行楷", 30, 70));
    QString grade = QString("分数:%1").arg(score);
    gamePainter->drawText(20, 60, grade);

    //格子
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            if(lattice[i][j] == 0)
            {
                gamePainter->drawPixmap(7 + i *65, 150 + j *65, QPixmap(":/new/image/0.png"));
            }
            else if(lattice[i][j] == 2)
            {
                forLattice(gamePainter, i, j, 2);
            }
            else if(lattice[i][j] == 4)
            {
                forLattice(gamePainter, i, j, 4);
            }
            else if(lattice[i][j] == 8)
            {
                forLattice(gamePainter, i, j, 8);
            }
            else if(lattice[i][j] == 16)
            {
                forLattice(gamePainter, i, j, 16);
            }
            else if(lattice[i][j] == 32)
            {
                forLattice(gamePainter, i, j, 32);
            }
            else if(lattice[i][j] == 64)
            {
                forLattice(gamePainter, i, j, 64);
            }
            else if(lattice[i][j] == 128)
            {
                forLattice(gamePainter, i, j, 128);
            }
            else if(lattice[i][j] == 256)
            {
                forLattice(gamePainter, i, j, 256);
            }
            else if(lattice[i][j] == 512)
            {
                forLattice(gamePainter, i, j, 512);
            }
            else if(lattice[i][j] == 1024)
            {
                forLattice(gamePainter, i, j, 1024);
            }
            else if(lattice[i][j] == 2048)
            {
                forLattice(gamePainter, i, j, 2048);
            }
            else
            {
                gamePainter->drawPixmap(7 + i *65, 150 + j *65, QPixmap(":/new/image/2048.png"));
                gamePainter->setFont(QFont("微软雅黑", 12, 70));
                gamePainter->drawText(QRect(7 + i *65, 150 + j *65, 60, 60), QString("%1").arg(lattice[i][j]), QTextOption(Qt::AlignCenter));
            }
        }
    }
    delete gamePainter;
}

void Widget::forLattice(QPainter *gamePainter, int i, int j, int numB)
{
    QString imageA = QString(":/new/image/%1.png").arg(numB);
    gamePainter->drawPixmap(7 + i *65, 150 + j *65, QPixmap(imageA));
    if(numB > 1000)
    {
        gamePainter->setFont(QFont("微软雅黑", 15, 70));
        gamePainter->setPen(QColor(255, 0, 0));
    }
    else if (numB > 100)
    {
        gamePainter->setFont(QFont("微软雅黑", 19, 70));
        gamePainter->setPen(QColor(0, 255, 0));
    }
    else if(numB > 10)
    {
        gamePainter->setFont(QFont("微软雅黑", 30, 70));
        gamePainter->setPen(QColor(0, 255, 255));
    }
    else if(numB > 0)
    {
        gamePainter->setFont(QFont("微软雅黑", 36, 70));
        gamePainter->setPen(QColor(234, 44, 235));
    }
    gamePainter->drawText(QRect(7 + i *65, 150 + j *65, 60, 60), QString("%1").arg(numB), QTextOption(Qt::AlignCenter));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(start == false)
    {
        return;
    }
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        PressUp();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        PressDown();
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        PressLeft();
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        pressRight();
        break;
    default:
        return;
    }
    randEnd();
    update();
}

void Widget::PressUp()
{
    //移动
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            if(lattice[i][j] == 0)
            {
                continue;
            }
            for(int k = 0; k < j; k++)
            {
                //查看前面是否有空格子可以移动
                if(lattice[i][k] == 0)
                {
                    lattice[i][k] = lattice[i][j];
                    lattice[i][j] = 0;
                    break;
                }
            }
        }
    }
    //相加
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(lattice[i][j] == lattice[i][j + 1] && lattice[i][j] != 2048)
            {
                lattice[i][j] = lattice[i][j] *2;
                lattice[i][j + 1] = 0;
                score +=lattice[i][j];
                for(int k = j + 2; k < 6; k++)
                {
                    lattice[i][k - 1] = lattice[i][k];
                }
            }
        }
    }
}

void Widget::PressDown()
{
    //移动
    for(int i = 0; i < 6; i++)
    {
        for(int j = 5; j >= 0; j--)
        {
            if(lattice[i][j] == 0)
            {
                continue;
            }
            for(int k = 5; k > j; k--)
            {
                //查看前面是否有空格子可以移动
                if(lattice[i][k] == 0)
                {
                    lattice[i][k] = lattice[i][j];
                    lattice[i][j] = 0;
                    break;
                }
            }
        }
    }
    //相加
    for(int i = 0; i < 6; i++)
    {
        for(int j = 5; j > 0; j--)
        {
            if(lattice[i][j] == lattice[i][j - 1] && lattice[i][j] != 2048)
            {
                lattice[i][j] = lattice[i][j] *2;
                lattice[i][j - 1] = 0;
                score +=lattice[i][j];
                for(int k = j - 2; k >=0; k--)
                {
                    lattice[i][k + 1] = lattice[i][k];
                }
            }
        }
    }
}

void Widget::PressLeft()
{
    //移动
    for(int j = 0; j < 6; j++)
    {
        for(int i = 0; i < 6; i++)
        {
            if(lattice[i][j] == 0)
            {
                continue;
            }
            for(int k = 0; k < i; k++)
            {
                //查看前面是否有空格子可以移动
                if(lattice[k][j] == 0)
                {
                    lattice[k][j] = lattice[i][j];
                    lattice[i][j] = 0;
                    break;
                }
            }
        }
    }
    //相加
    for(int j = 0; j < 6; j++)
    {
        for(int i = 0; i < 5; i++)
        {
            if(lattice[i][j] == lattice[i + 1][j] && lattice[i][j] != 2048)
            {
                lattice[i][j] = lattice[i][j] *2;
                lattice[i + 1][j] = 0;
                score +=lattice[i][j];
                for(int k = i + 2; k < 6; k++)
                {
                    lattice[k - 1][j] = lattice[k][j];
                }
            }
        }
    }
}

void Widget::pressRight()
{
    //移动
    for(int j = 0; j < 6; j++)
    {
        for(int i = 5; i >= 0; i--)
        {
            if(lattice[i][j] == 0)
            {
                continue;
            }
            for(int k = 5; k > i; k--)
            {
                //查看前面是否有空格子可以移动
                if(lattice[k][j] == 0)
                {
                    lattice[k][j] = lattice[i][j];
                    lattice[i][j] = 0;
                    break;
                }
            }
        }
    }
    //相加
    for(int j = 0; j < 6; j++)
    {
        for(int i = 5; i > 0; i--)
        {
            if(lattice[i][j] == lattice[i - 1][j] && lattice[i][j] != 2048)
            {
                lattice[i][j] = lattice[i][j] *2;
                lattice[i - 1][j] = 0;
                score +=lattice[i][j];
                for(int k = i - 2; k >= 0; k--)
                {
                    lattice[k + 1][j] = lattice[k][j];
                }
            }
        }
    }
}

void Widget::randEnd()
{
    //找出格子
    struct Residue res[36];
    int sur = 0;//剩余空格子
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            if(lattice[i][j] == 0)
            {
                res[sur].i = i;
                res[sur].j = j;
                sur++;
            }
        }
    }

    //判断游戏结束
    if(sur == 0)
    {
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < 6; j++)
            {
                if(lattice[i][j] == lattice[i][j + 1] && lattice[i][j] != 2048)
                {
                    return;
                }
            }
        }
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < 6; j++)
            {
                if(lattice[i][j] == lattice[i][j - 1] && lattice[i][j] != 2048)
                {
                    return;
                }
            }
        }
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < 6; j++)
            {
                if(lattice[i][j] == lattice[i + 1][j] && lattice[i][j] != 2048)
                {
                    return;
                }
            }
        }
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < 6; j++)
            {
                if(lattice[i][j] == lattice[i - 1][j] && lattice[i][j] != 2048)
                {
                    return;
                }
            }
        }
        QMessageBox::about(this,"游戏失败","你的分数为:" + QString::number(score) + "分");
        return;
    }
    //在随机的格子里，生成一个2
    int rand = qrand()%sur;
    lattice[res[rand].i][res[rand].j] = 2;
}
