#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(int _SIZE,QWidget *parent) :
    QMainWindow(parent),SIZE(_SIZE),
    ui(new Ui::MainWindow)
{
    Mainmap=new cell::cellMap(SIZE);
    Mainmap->loadMap(0.5,time(0));
    Mainmap->startMap();
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    painter=new QPainter;
    painter->begin(this);
    painter->setPen(QPen(Qt::darkGreen,0.2,Qt::SolidLine));
    for(int i=0;i<=SIZE;++i)
    {
        painter->drawLine(Ox,Oy+WIDTH*i,Ox+WIDTH*SIZE,Oy+WIDTH*i);
        painter->drawLine(Ox+WIDTH*i,Oy,Ox+WIDTH*i,Oy+WIDTH*SIZE);
    }
}

void MainWindow::drawTheArr()
{
    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    for(int i=0;i<SIZE;++i)
    {
        for(int j=0;j<SIZE;++j)
        {
            if(Mainmap->cget(cell::position(i,j)).state==cell::LIVE)
            {
                painter->drawEllipse(Ox+WIDTH*i,Oy+WIDTH*j,WIDTH/2,WIDTH/2);
            }
        }
    }
}


