#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //目前地图的大小是写死的，根据需要以后会设计改变。
    MainWindow w(50, 40);
    w.showMaximized();

    return a.exec();
}
