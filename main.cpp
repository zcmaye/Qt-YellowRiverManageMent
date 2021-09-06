#include "MainWindow/controller.h"

#include <QApplication>
#include<QStyleFactory>
#include<QDebug>
#include<QStyle>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStyle *style =  QStyleFactory::create("Fusion");
    a.setStyle(style);
    qDebug()<<QStyleFactory::keys();    //("windowsvista", "Windows", "Fusion")
    Controller c;
    return a.exec();
}

