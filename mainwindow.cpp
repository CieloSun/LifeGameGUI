#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QMessageBox>

MainWindow::MainWindow(int _width,int _height,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    MapWidth=_width;
    MapHeight=_height;
    WIDTH=this->width()/MapWidth;
    Ox=this->width()/7;
    Oy=this->height()/10;
    myTimerId=0;


    Mainmap=new cell::cellMap(MapWidth,MapHeight);
    Mainmap->loadMap(0.5,time(0));
    cell::cellMap::startMap(Mainmap);

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
    painter->setPen(QPen(Qt::darkGreen,0.2,Qt::DotLine));
    for(int i=0;i<=MapHeight;++i)
    {
        painter->drawLine(Ox,Oy+WIDTH*i,Ox+WIDTH*MapWidth,Oy+WIDTH*i);
    }
    for(int i=0;i<=MapWidth;++i)
    {
        painter->drawLine(Ox+WIDTH*i,Oy,Ox+WIDTH*i,Oy+WIDTH*MapHeight);
    }

    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            if(Mainmap->cget(i,j).getState()==cell::cell::LIVE)
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
    /*
    fstream file("savedata.dat");
    if(file)
    {
        Mainmap->saveMap(file);
        QMessageBox::information(this,"Hint",QString::fromStdString("You have save it successfully"),QMessageBox::Cancel);
    }
    file.close();
    */
}

void MainWindow::Load()
{
    /*
    fstream file("savedata.dat");
    if(file)
    {
        Mainmap->loadMap(file);
        QMessageBox::information(this,"Hint",QString::fromStdString("You have load it successfully"),QMessageBox::Cancel);
    }
    else QMessageBox::critical(this,"Error",QString::fromStdString("No save data"),QMessageBox::Cancel);
    file.close();
    */
}



