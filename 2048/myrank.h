#ifndef MYRANK_H
#define MYRANK_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QTableWidget>
#include "mypushbutton.h"

class MyRank : public QWidget
{
    Q_OBJECT
public:
    explicit MyRank(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    MyPushButton *buttonReturn;

signals:
    void signalReturn();

public slots:
private:
    QTableWidget *myTable;
};

#endif // MYRANK_H
