#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QMessageBox>

MainWindow::MainWindow(int _SIZE,QWidget *parent) :
    QMainWindow(parent),SIZE(_SIZE),
    ui(new Ui::MainWindow)
{
    WIDTH=this->width()/SIZE;
    Ox=this->width()/7;
    Oy=this->height()/10;
    myTimerId=0;


    Mainmap=new cell::cellMap(SIZE);
    Mainmap->loadMap(0.5,time(0));
    Mainmap->startMap();

    ui->setupUi(this);

    connect(ui->actionStart,SIGNAL(triggered(bool)),this,SLOT(Start()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
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
        painter->drawLine(Ox,Oy+WIDTH*i,Ox+WIDTH*SIZE*2,Oy+WIDTH*i);
    }
    for(int i=0;i<=2*SIZE;++i)
    {
        painter->drawLine(Ox+WIDTH*i,Oy,Ox+WIDTH*i,Oy+WIDTH*SIZE);
    }

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

void MainWindow::Start()
{
    myTimerId=startTimer(30);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==myTimerId)
    {
        update();
        updateGeometry();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void MainWindow::Save()
{
    fstream file("savedata.dat");
    if(file)
    {
        cell::cellMap::saveMap(file);
        QMessageBox::information(this,"Hint",QString::fromStdString("You have save it successfully"),QMessageBox::Cancel);
    }
    file.close();
}

void MainWindow::Load()
{
    fstream file("savedata.dat");
    if(file)
    {
        cell::cellMap::loadMap(file);
        QMessageBox::information(this,"Hint",QString::fromStdString("You have load it successfully"),QMessageBox::Cancel);
    }
    else QMessageBox::critical(this,"Error",QString::fromStdString("No save data"),QMessageBox::Cancel);
    file.close();
}



