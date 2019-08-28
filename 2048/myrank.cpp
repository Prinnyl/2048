#include "myrank.h"
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QPainter>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QHeaderView>
#include <QTimer>

MyRank::MyRank(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(400, 700);
    this->setWindowTitle("排行榜");

    myTable = new QTableWidget(this);//表格
    myTable->setFixedSize(400, 700);

    myTable->setColumnCount(2);//设置列数
    myTable->setRowCount(9);//设置行数

    //设置行高列宽
    for(int columns=0;columns<2;++columns)
    {
        for(int rows=0;rows<10;++rows)
        {
            myTable->setColumnWidth(columns,100);
            myTable->setRowHeight(rows,50);
        }
    }
    //myTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//垂直滚动
    QStringList mHeader;
    mHeader << QString("分数") << QString("时间");
    myTable->setHorizontalHeaderLabels(mHeader);//添加横向表头
    myTable->horizontalHeader()->setStretchLastSection(true);//最后一个单元格扩展
    myTable->setShowGrid(false);//栅格隐藏
    myTable->setFocusPolicy(Qt::NoFocus);//解决选中虚框问题
    myTable->setSelectionMode(QAbstractItemView::NoSelection);
    myTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表

    //设置横表头字体，颜色，模式
    myTable->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color : rgba(255, 255, 255, 0);"
                                               "color : black;"
                                               "font-size : 27px;"
                                               "font-family : 华文行楷;"
                                               "border : 0px;"
                                               "font-weight : bold;}");

    //设置竖表头字体，颜色，模式
    myTable->verticalHeader()->setStyleSheet("QHeaderView::section {background-color : rgba(255, 255, 255, 0);"
                                             "color : black;"
                                             "margin-left : 9px;"
                                             "font-size : 24px;"
                                             "border : 0px;"
                                             "font-weight : bold;}");
    //设置表格样式
    myTable->setStyleSheet("background-color : rgba(255, 255, 255, 0);"
                           "color : rgb(255, 0, 0);"
                           "font-size : 20px;"
                           "font-family : 微软雅黑;"
                           "font-weight : bold;");

    //添加数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    //连接数据库
    //db.setHostName("127.0.0.1");
    //db.setPort(3306);
    //db.setUserName("root");
    //db.setPassword("53469");
    db.setDatabaseName("prinny");

    //打开数据库
    if(db.open() == false)
    {
        QMessageBox::warning(this, "错误", db.lastError().text());
    }
    QSqlQuery query;
    query.exec("select * from scoreRank order by scoref desc");
    int number = 0;
    while(query.next())
    {
//        qDebug() << query.value(0).toInt()
//                 << query.value(1).toString()
//                 << query.value("timef").toString();
        myTable->setItem(number, 0,new QTableWidgetItem(QString("%1").arg(query.value("scoref").toString())));
        myTable->setItem(number, 1,new QTableWidgetItem(QString("%1").arg(query.value("timef").toString())));
        number++;
    }
    if(number > 9)
    {
        number = 9;
    }
    myTable->setRowCount(number);//设置行数

    //返回按钮
    buttonReturn = new MyPushButton(":/new/image/return.png", 1);
    buttonReturn->setParent(this);
    buttonReturn->move(50, 550);
    connect(buttonReturn, &MyPushButton::clicked, [=](){
        buttonReturn->zoom1();
        buttonReturn->zoom2();
        QTimer::singleShot(200, this, [=](){
            emit signalReturn();
        });
    });
}

void MyRank::paintEvent(QPaintEvent *)
{
    //背景
    QPainter groundPainter(this);
    groundPainter.drawPixmap(0, 0, QPixmap(":/new/image/57526556_p0.png"));
}
