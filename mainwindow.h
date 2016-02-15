#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include "cell.h"
#include "ui_mainwindow.h"

namespace cell {
class MainWindow;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int _SIZE,QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
private slots:
    void Start();
private:

    Ui::MainWindow *ui;
    QPainter *painter;
    cell::cellMap *Mainmap;
    int myTimerId;
    int SIZE;
    int WIDTH;
    int Ox;
    int Oy;
};

#endif // MAINWINDOW_H
