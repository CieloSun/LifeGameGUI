#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enddialog.h"
#include "savedialog.h"
#include "myrestartdialog.h"
#include <QFile>
#include <QString>
#include <fstream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QToolTip>

MainWindow::MainWindow(int _width, int _height, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    have_run_times = 0;
    MapWidth = _width;
    MapHeight = _height;
    WIDTH = 50;
    Ox = 8;
    Oy = 30;

    ui->setupUi(this);

    Mainmap = new cell::cellMap(MapWidth, MapHeight);
    threadRun = new Thread(Mainmap);

    connect(threadRun, SIGNAL(ChangeScreen()), this, SLOT(Change()));
    connect(threadRun, SIGNAL(End()), this, SLOT(End()));
    connect(ui->actionStart, SIGNAL(triggered(bool)), this, SLOT(Start()));
    connect(ui->actionRestart, SIGNAL(triggered(bool)), this, SLOT(Restart()));
    connect(ui->actionSetting, SIGNAL(triggered(bool)), this, SLOT(Setting()));
    connect(ui->actionPause, SIGNAL(triggered(bool)), this, SLOT(Stop()));
    connect(ui->actionEnd, SIGNAL(triggered(bool)), this, SLOT(End()));
    connect(ui->actionResume, SIGNAL(triggered(bool)), this, SLOT(Resume()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(Save()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(Load()));
    connect(ui->actionTest, SIGNAL(triggered(bool)), this, SLOT(Test()));

}


void MainWindow::ReStartFunction(int _sp, double p_N, double c_N, double h_N)
{
    have_run_times = 0;
    threadRun->restart(_sp, p_N, c_N, h_N);
}

void MainWindow::Restart()
{
    int speed_v;
    sdialog = new MyRestartDialog();
    connect(sdialog->restartButton, SIGNAL(clicked()), sdialog, SLOT(accept()));

    if (sdialog->exec() == QDialog::Accepted)
    {
        //threadRun->stop();
        int speed_text = sdialog->speedComboBox->currentIndex();
        if (speed_text == 1)
        {
            speed_v = 500;
        }
        else if (speed_text == 2)
        {
            speed_v = 1000;
        }
        else if (speed_text == 3)
        {
            speed_v = 100;
        }

        double producer_f = sdialog->producerSpinBox->value();
        double consumer_f = sdialog->consumerSpinBox->value();
        double highConsumer_f = sdialog->highSpinBox->value();
        ReStartFunction(speed_v, producer_f, consumer_f, highConsumer_f);

    }
    have_run_times++;
    //利用对话框来获取数据调用RestartFunction(int,int,int,doule,double,double)
}


void MainWindow::Change()
{
    update();
    updateGeometry();
    have_run_times++;
}

void MainWindow::Start()
{
    threadRun->start();
}



void MainWindow::Stop()
{
    if (threadRun->isRunning())
    {
        threadRun->stop();
    }
}

void MainWindow::Resume()
{
    threadRun->resume();
    threadRun->start();
}

void MainWindow::Test()
{
    threadRun->resume();
    threadRun->start();
    Sleep(1000);
    if (threadRun->isRunning())
    {
        threadRun->stop();
    }
}

void MainWindow::SaveFunction(QString fileName)
{
    std::fstream setup(fileName.toStdString(), std::ios::out);
    setup.close();
    std::fstream out(fileName.toStdString());
    out << MapWidth << '\t' << MapHeight << '\n';
    for (int i = 0; i < MapWidth; ++i)
    {
        for (int j = 0; j < MapHeight; ++j)
        {
            out << Mainmap->cget(i, j).getType() << '\t' << Mainmap->cget(i, j).getState() << '\t'
                << Mainmap->cget(i, j).getRange() << '\t' << Mainmap->cget(i, j).getLiveNumber() << '\t'
                << Mainmap->cget(i, j).getDeadNumber() << '\t' << Mainmap->cget(i, j).getAgeLimit() << '\t'
                << Mainmap->cget(i, j).getAge() << '\t' << Mainmap->cget(i, j).getAfterDeadLimit() << '\t'
                << Mainmap->cget(i, j).getAfterDead() << '\t'<<Mainmap->cget(i, j).getProduceAge() << '\n';
        }
    }
}

void MainWindow::Save()
{
    QString filename = QFileDialog::getSaveFileName(this);
    SaveFunction(filename);
}
void MainWindow::LoadFunction(QString fileName)
{
    std::fstream in(fileName.toStdString());
    in >> MapWidth >> MapHeight;
    int _type, _state, _range, _liveNumber, _deadNumber, _ageLimit, _age, _afterDeadLimit, _afterDead,_produceAge;
    for (int i = 0; i < MapWidth; ++i)
    {
        for (int j = 0; j < MapHeight; ++j)
        {
            in >> _type >> _state >> _range >> _liveNumber >> _deadNumber >> _ageLimit
                    >> _age >> _afterDeadLimit >> _afterDead>>_produceAge;
            Mainmap->cget(i, j).setType(_type);
            Mainmap->cget(i, j).setState(_state);
            Mainmap->cget(i, j).setRange(_range);
            Mainmap->cget(i, j).setLiveNumber(_liveNumber);
            Mainmap->cget(i, j).setDeadNumber(_deadNumber);
            Mainmap->cget(i, j).setAgeLimit(_ageLimit);
            Mainmap->cget(i, j).setAge(_age);
            Mainmap->cget(i, j).setAfterDeadLimit(_afterDeadLimit);
            Mainmap->cget(i, j).setAfterDead(_afterDead);
            Mainmap->cget(i, j).setProduceAge(_produceAge);
        }
    }
    update();
    updateGeometry();
}

void MainWindow::Load()
{
    SaveFunction(QString::fromStdString("savedata"));
    QString name = QFileDialog::getOpenFileName(this);
    QString filename = QFileInfo(name).fileName();
    if (!filename.size())
    {
        filename = QString::fromStdString("savedata");
    }
    LoadFunction(filename);
}

void MainWindow::End()
{
    MyEndDialog *edialog = new MyEndDialog();
    if (threadRun->isRunning())
    {
        threadRun->stop();
    }
    edialog->show();

    //TODO
    //调出一个结果统计报告页面并询问是否存储
    //报告包括每个物种的现存数量，空地的数量，总运行次数,建议以一个对话框显示，并提供退出和重新开始两个按钮
    //如果想实时获取数据，可以把这四个变量改为类变量
    int nothing_number = threadRun->getNothingNumber();
    int producer_number = threadRun->getProducerNumber();
    int consumer_number = threadRun->getConsumerNumber();
    int high_consumer_number = threadRun->getHighConsumerNumber();

    QString nothing_text = QString::number(nothing_number);
    edialog->nothingShow->setText(nothing_text);
    QString producer_text = QString::number(producer_number);
    edialog->producerShow->setText(producer_text);
    QString consumer_text = QString::number(consumer_number);
    edialog->consumerShow->setText(consumer_text);
    QString high_consumer_text = QString::number(high_consumer_number);
    edialog->highShow->setText(high_consumer_text);

    connect(edialog->yesButton, SIGNAL(clicked()), edialog, SLOT(save()));
    connect(edialog->noButton, SIGNAL(clicked()), edialog, SLOT(close()));
}


void MainWindow::ChangeByUser(int selection)
{
    //TODO
    //通过selection的不同来对环境进行不同的设置，是否使用你们发挥一下
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //TODO
    //利用点击时的坐标（找出一种算法）来反向计算出其表示的数组对应index，利用浮出提示/信息栏来显示出对应的信息
    //调用信息可以使用cell类中的各种get函数
    int x=(event->pos().rx()-Ox)/WIDTH;
    int y=(event->pos().ry()-Oy)/WIDTH;
    if(x>=0&&x<=MapWidth&&y>=0&&y<=MapHeight)
    {
        //_type, _state, _range, _liveNumber, _deadNumber, _ageLimit, _age, _afterDeadLimit, _afterDead,_produceAge;
        QString typeString="Type: ";
        switch(Mainmap->cget(x,y).getType())
        {
        case cell::NOTHING:
            typeString+="Nothing";
            break;
        case cell::PRODUCER:
            typeString+="Producer";
            break;
        case cell::CONSUMER:
            typeString+="Consumer";
            break;
        case cell::HIGH_CONSUMER:
            typeString+="High Consumer";
            break;
        default:
            typeString+="Unknown";
            break;
        }
        QString stateString="\nState: ";
        switch(Mainmap->cget(x,y).getState())
        {
        case cell::EMPTY:
            stateString+="Nothing";
            break;
        case cell::LIVE:
            stateString+="Live";
            break;
        case cell::DEAD:
            stateString+="Dead";
            break;
        default:
            stateString+="Unknown";
            break;
        }
        QString rangeString="\nRange: ";
        rangeString+=QString::number(Mainmap->cget(x,y).getRange(),10);
        QString liveNumberString="\nLive Number: ";
        liveNumberString+=QString::number(Mainmap->cget(x,y).getLiveNumber(),10);
        QString deadNumberString="\nDead Number: ";
        deadNumberString+=QString::number(Mainmap->cget(x,y).getDeadNumber(),10);
        QString ageLimitString="\nAge Limit: ";
        ageLimitString+=QString::number(Mainmap->cget(x,y).getAgeLimit(),10);
        QString ageString="\nAge: ";
        ageString+=QString::number(Mainmap->cget(x,y).getAge(),10);
        QString afterDeadLimitString="\nAfter Dead Limit: ";
        afterDeadLimitString+=QString::number(Mainmap->cget(x,y).getAfterDeadLimit(),10);
        QString afterDeadString="\nDead Years: ";
        afterDeadString+=QString::number(Mainmap->cget(x,y).getAfterDead(),10);
        QString produceAgeString="\nGrow up Age: ";
        produceAgeString+=QString::number(Mainmap->cget(x,y).getProduceAge(),10);
        QToolTip::showText(event->pos(),typeString+stateString+rangeString+liveNumberString+deadNumberString
                           +ageLimitString+ageString+afterDeadLimitString+afterDeadString+produceAgeString);
    }

}


void MainWindow::Setting()
{
    int speed_v;
    sdialog = new MyRestartDialog();
    sdialog->restartButton->setText("Set");
    connect(sdialog->restartButton, SIGNAL(clicked()), sdialog, SLOT(accept()));

    if (sdialog->exec() == QDialog::Accepted)
    {
        //threadRun->stop();
        int speed_text = sdialog->speedComboBox->currentIndex();
        if (speed_text == 1)
        {
            speed_v = 500;
        }
        else if (speed_text == 2)
        {
            speed_v = 1000;
        }
        else if (speed_text == 3)
        {
            speed_v = 100;
        }

        double producer_f = sdialog->producerSpinBox->value();
        double consumer_f = sdialog->consumerSpinBox->value();
        double highConsumer_f = sdialog->highSpinBox->value();
        ReStartFunction(speed_v, producer_f, consumer_f, highConsumer_f);
    }
    //TODO
    //同样是利用对话框（甚至是和Restart同一个对话框）来调用RestartFunction即可。
}


void MainWindow::paintEvent(QPaintEvent *)
{
    painter = new QPainter;
    painter->begin(this);
    painter->setPen(QPen(Qt::darkGreen, WIDTH / 20, Qt::DotLine));
    for (int i = 0; i <= MapHeight; ++i)
    {
        painter->drawLine(Ox, Oy + WIDTH * i, Ox + WIDTH * MapWidth, Oy + WIDTH * i);
    }
    for (int i = 0; i <= MapWidth; ++i)
    {
        painter->drawLine(Ox + WIDTH * i, Oy, Ox + WIDTH * i, Oy + WIDTH * MapHeight);
    }


    for (int i = 0; i < MapWidth; ++i)
    {
        for (int j = 0; j < MapHeight; ++j)
        {
            QPixmap pixmap;
            pixmap.load(":image/ground.png");
            painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH, WIDTH, pixmap);
            if (Mainmap->cget(i, j).getState() == cell::LIVE)
            {

                if (Mainmap->cget(i, j).getType() == cell::PRODUCER)
                {
                    QPixmap pixmap;
                    pixmap.load(":/image/glass_3.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.7, WIDTH * 0.7, pixmap);
                }
                else if (Mainmap->cget(i, j).getType() == cell::CONSUMER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/mouse.ico");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.8, WIDTH * 0.8, pixmap);
                }
                else if (Mainmap->cget(i, j).getType() == cell::HIGH_CONSUMER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/wolf.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH, WIDTH, pixmap);
                }

            }
            else if (Mainmap->cget(i, j).getState() == cell::DEAD)
            {
                if (Mainmap->cget(i, j).getType() == cell::PRODUCER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/dead_leaf_2.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.6, WIDTH * 0.6, pixmap);
                }
                else if (Mainmap->cget(i, j).getType() == cell::CONSUMER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/tomb.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.6, WIDTH * 0.6, pixmap);
                }
                else if (Mainmap->cget(i, j).getType() == cell::HIGH_CONSUMER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/skull.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.6, WIDTH * 0.6, pixmap);
                }
            }
        }

    }

    painter->end();
}

MainWindow::~MainWindow()
{
    delete ui;
}
