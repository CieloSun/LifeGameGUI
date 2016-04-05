#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //目前地图的大小是写死的，根据需要以后会设计改变。
    MainWindow w(27, 13);
    w.showMaximized();

    QFile styleFile(":/style.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString setStyleSheet(styleFile.readAll());;
    a.setStyleSheet(setStyleSheet);

    return a.exec();
}
