#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "stdafx.h"
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
    const int WIDTH=10;
    const int Ox=20;
    const int Oy=50;

    explicit MainWindow(int _SIZE,QWidget *parent = 0);
    ~MainWindow();
private slots:
    void drawTheArr();
private:
    void paintEvent(QPaintEvent *);
    Ui::MainWindow *ui;
    QPainter *painter;
    cell::cellMap *Mainmap;
    int SIZE;
};

#endif // MAINWINDOW_H
