#include "mainwindow.h"
#include <QApplication>

const int defaultWidth=50;
const int defaultHeight=50;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //目前地图的大小是写死的，根据需要以后会设计改变。
    MainWindow w(defaultWidth,defaultHeight);
    w.show();

    return a.exec();
}
