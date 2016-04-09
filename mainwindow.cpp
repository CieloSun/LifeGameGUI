#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enddialog.h"
#include "myrestartdialog.h"
#include "settingdialog.h"
#include <QFile>
#include <QString>
#include <fstream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QToolTip>
#include <random>
#include <QPushButton>


MainWindow::MainWindow(int _width, int _height, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    damage=false;
    have_run_times = 0;
    MapWidth = _width;
    MapHeight = _height;
    WIDTH = 50;
    Ox = 30;
    Oy = 30;

    ui->setupUi(this);

    Mainmap = new cell::cellMap(MapWidth, MapHeight);
    threadRun = new Thread(Mainmap);

    //connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(Start()));

    connect(threadRun, SIGNAL(ChangeScreen()), this, SLOT(Change()));
    connect(threadRun, SIGNAL(End()), this, SLOT(End()));
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(Start()));
    connect(ui->restartButton, SIGNAL(clicked(bool)), this, SLOT(Restart()));
    connect(ui->setButton, SIGNAL(clicked(bool)), this, SLOT(Setting()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(Stop()));
    connect(ui->endButton, SIGNAL(clicked(bool)), this, SLOT(End()));
    connect(ui->resumeButton, SIGNAL(clicked(bool)), this, SLOT(Resume()));
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(Save()));
    connect(ui->openButton, SIGNAL(clicked(bool)), this, SLOT(Load()));
    connect(ui->damageButton,SIGNAL(clicked(bool)),this,SLOT(damageFunction()));
    connect(ui->fineButton,SIGNAL(clicked(bool)),this,SLOT(fineFunction()));
    //connect(ui->actionTest, SIGNAL(triggered(bool)), this, SLOT(Test()));

    //QWidget *widget=new QWidget();
    //parent->setAutoFillBackground(true);
    QPalette palette;
    //QPixmap pixmap(":image/bg3.png");
    palette.setColor(QPalette::Background,QColor(188,209,202));
    this->setPalette(palette);
    this->show();

    ui->startButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->pauseButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->resumeButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->restartButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->endButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->setButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->damageButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->fineButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->groupBox->setStyleSheet("color:#ffffff;border-radius:5px;border:1px outset #ffffff" );

    ui->saveButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->openButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->aboutButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->exitButton->setStyleSheet("background-color:#ffffff;color:#bcd1ca;border:0px;border-radius:5px;");
    ui->groupBox2->setStyleSheet("color:#ffffff;border-radius:5px;border:1px outset #ffffff");

    //    QDialogButtonBox *dialogButtonBox;
    //    dialogButtonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    //    dialogButtonBox->button(QDialogButtonBox::Ok)->setText("执行");
    //    dialogButtonBox->button(QDialogButtonBox::Cancel)->setText("放弃");
}


void MainWindow::ReStartFunction(double p_N, double c_N, double h_N)
{
    have_run_times = 0;
    threadRun->restart(p_N, c_N, h_N);
    //threadRun->start();
}

void MainWindow::Restart()
{

    sdialog = new MyRestartDialog();
    connect(sdialog->restartButton, SIGNAL(clicked()), sdialog, SLOT(accept()));

    if (sdialog->exec() == QDialog::Accepted)
    {
        //threadRun->stop();

        int speed_v;
        int speed_text = sdialog->speedComboBox->currentIndex();
        if (speed_text == 0)
        {
            speed_v = cell::NORMAL_SPEED;
        }
        else if (speed_text == 1)
        {
            speed_v = cell::SLOW_SPEED;
        }
        else if (speed_text == 2)
        {
            speed_v = cell::FAST_SPEED;
        }
        Mainmap->setSpeed(speed_v);



        double producer_f = sdialog->producerSpinBox->value();
        double consumer_f = sdialog->consumerSpinBox->value();
        double highConsumer_f = sdialog->highSpinBox->value();
        ReStartFunction(producer_f, consumer_f, highConsumer_f);
        //threadRun->resume();

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
    std::ofstream out(fileName.toStdString());
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            out<<Mainmap->cget(i,j).getType()<<'\t';
            out<<Mainmap->cget(i,j).getState()<<'\t';
            out<<Mainmap->cget(i,j).getRange()<<'\t';
            out<<Mainmap->cget(i,j).getLiveNumber()<<'\t';
            out<<Mainmap->cget(i,j).getDeadNumber()<<'\t';
            out<<Mainmap->cget(i,j).getAge()<<'\t';
            out<<Mainmap->cget(i,j).getAfterDead()<<'\t';
            out<<Mainmap->cget(i,j).getProduceAge()<<'\t';
            out<<Mainmap->cget(i,j).getStarvingTime()<<'\t';
            out<<Mainmap->cget(i,j).getAgeLimit()<<'\t';
            out<<Mainmap->cget(i,j).getAfterDeadLimit()<<'\t';
            out<<Mainmap->cget(i,j).getStarvingTimeLimit()<<'\n';
        }
    }
    out.close();
}
void MainWindow::Save()
{
    QString filename = QFileDialog::getSaveFileName(this);
    SaveFunction(filename);
}
void MainWindow::LoadFunction(QString fileName)
{
    std::ifstream in(fileName.toStdString());
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            int _type,_state,_range,_liveNumber,_deadNumber,_age,
                    _afterDead,_produceAge,_starvingTime,_ageLimit,_deadLimit,_starvingTimeLimit;
            in>>_type>>_state>>_range>>_liveNumber>>_deadNumber>>_age
                    >>_afterDead>>_produceAge>>_starvingTime>>_ageLimit>>_deadLimit>>_starvingTimeLimit;
            Mainmap->cget(i,j).setType(_type);
            Mainmap->cget(i,j).setState(_state);
            Mainmap->cget(i,j).setRange(_range);
            Mainmap->cget(i,j).setLiveNumber(_liveNumber);
            Mainmap->cget(i,j).setDeadNumber(_deadNumber);
            Mainmap->cget(i,j).setAge(_age);
            Mainmap->cget(i,j).setAfterDead(_afterDead);
            Mainmap->cget(i,j).setProduceAge(_produceAge);
            Mainmap->cget(i,j).setStarvingTime(_starvingTime);
            Mainmap->cget(i,j).setAgeLimit(_ageLimit);
            Mainmap->cget(i,j).setAfterDeadLimit(_deadLimit);
            Mainmap->cget(i,j).setStarvingTimeLimit(_starvingTimeLimit);
        }
    }

    in.close();
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
        std::cerr<<"Successful!"<<std::endl;
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
    QString have_run_times_text = QString::number(have_run_times);
    edialog->runShow->setText(have_run_times_text);

    connect(edialog->yesButton, SIGNAL(clicked()), this, SLOT(Restart()));
    connect(edialog->yesButton, SIGNAL(clicked()), edialog, SLOT(close()));
    connect(edialog->noButton, SIGNAL(clicked()), edialog, SLOT(close()));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //TODO
    //利用点击时的坐标（找出一种算法）来反向计算出其表示的数组对应index，利用浮出提示/信息栏来显示出对应的信息
    //调用信息可以使用cell类中的各种get函数
    int x = (event->pos().rx() - Ox) / WIDTH;
    int y = (event->pos().ry() - Oy) / WIDTH;
    if (x >= 0 && x <= MapWidth && y >= 0 && y <= MapHeight)
    {
        //_type, _state, _range, _liveNumber, _deadNumber, _ageLimit, _age, _afterDeadLimit, _afterDead,_produceAge;
        QString typeString = "Type: ";
        switch (Mainmap->cget(x, y).getType())
        {
        case cell::NOTHING:
            typeString += "Nothing";
            break;
        case cell::PRODUCER:
            typeString += "Producer";
            break;
        case cell::CONSUMER:
            typeString += "Consumer";
            break;
        case cell::HIGH_CONSUMER:
            typeString += "High Consumer";
            break;
        default:
            typeString += "Unknown";
            break;
        }
        QString stateString = "\nState: ";
        switch (Mainmap->cget(x, y).getState())
        {
        case cell::EMPTY:
            stateString += "Nothing";
            break;
        case cell::LIVE:
            stateString += "Live";
            break;
        case cell::DEAD:
            stateString += "Dead";
            break;
        default:
            stateString += "Unknown";
            break;
        }
        QString rangeString = "\nRange: ";
        rangeString += QString::number(Mainmap->cget(x, y).getRange(), 10);
        QString liveNumberString = "\nLive Number: ";
        liveNumberString += QString::number(Mainmap->cget(x, y).getLiveNumber(), 10);
        QString deadNumberString = "\nDead Number: ";
        deadNumberString += QString::number(Mainmap->cget(x, y).getDeadNumber(), 10);
        QString ageLimitString = "\nAge Limit: ";
        ageLimitString += QString::number(Mainmap->cget(x, y).getAgeLimit(), 10);
        QString ageString = "\nAge: ";
        ageString += QString::number(Mainmap->cget(x, y).getAge(), 10);
        QString afterDeadLimitString = "\nAfter Dead Limit: ";
        afterDeadLimitString += QString::number(Mainmap->cget(x, y).getAfterDeadLimit(), 10);
        QString afterDeadString = "\nDead Years: ";
        afterDeadString += QString::number(Mainmap->cget(x, y).getAfterDead(), 10);
        QString produceAgeString = "\nGrow up Age: ";
        produceAgeString += QString::number(Mainmap->cget(x, y).getProduceAge(), 10);
        QString starvingTimeLimitString = "\nStarving Limit: ";
        starvingTimeLimitString += QString::number(Mainmap->cget(x, y).getStarvingTimeLimit(), 10);
        QString starvingTimeString = "\nStarving Time: ";
        starvingTimeString += QString::number(Mainmap->cget(x, y).getStarvingTime(), 10);
        QToolTip::showText(event->pos(), typeString + stateString + rangeString + liveNumberString + deadNumberString
                           + ageLimitString + ageString + afterDeadLimitString + afterDeadString + produceAgeString
                           + starvingTimeLimitString + starvingTimeString);
    }

}


void MainWindow::Setting()
{
    SettingDialog *settingdialog = new SettingDialog();
    bool flag=false;


    if (threadRun->isRunning())
    {
        flag=true;
        threadRun->stop();
    }
    settingdialog->show();
    int nothing_number = threadRun->getNothingNumber();
    int producer_number = threadRun->getProducerNumber();
    int consumer_number = threadRun->getConsumerNumber();
    int high_consumer_number = threadRun->getHighConsumerNumber();


    QString nothing_text = QString::number(nothing_number);
    settingdialog->nothingShow->setText(nothing_text);
    QString producer_text = QString::number(producer_number);
    settingdialog->producerShow->setText(producer_text);
    QString consumer_text = QString::number(consumer_number);
    settingdialog->consumerShow->setText(consumer_text);
    QString high_consumer_text = QString::number(high_consumer_number);
    settingdialog->highShow->setText(high_consumer_text);
    QString have_run_times_text = QString::number(have_run_times);
    settingdialog->runShow->setText(have_run_times_text);


    if(settingdialog->exec()==QDialog::Accepted)
    {

        int speed_text = settingdialog->speedComboBox->currentIndex();
        if (speed_text == 0)
        {
            Mainmap->setSpeed(cell::NORMAL_SPEED);
        }
        else if (speed_text == 1)
        {
            Mainmap->setSpeed(cell::SLOW_SPEED);
        }
        else if (speed_text == 2)
        {
            Mainmap->setSpeed(cell::FAST_SPEED);
        }

        int producerAddNumber=settingdialog->ProducerBox->value();
        int consumerAddNumber=settingdialog->ConsumerBox->value();
        int highConsumerAddNumber=settingdialog->HighConsumerBox->value();
        int sum=producerAddNumber+consumerAddNumber+highConsumerAddNumber;

        double evolutionProbability=settingdialog->evolutionSpinBox->value();
        Mainmap->setEvolution(evolutionProbability);

        if(sum>nothing_number)
        {
            QMessageBox::critical(this,"Error","There is no enough room to put these.");
        }
        else
        {
            size_t seed=time(0)%10000;
            std::default_random_engine engine(seed);
            std::uniform_int_distribution<int> distribution(0, sum - 1);
            std::vector<int> cellVector;
            while(producerAddNumber-- > 0)
            {
                cellVector.push_back(cell::PRODUCER);
            }
            while(consumerAddNumber-- > 0)
            {
                cellVector.push_back(cell::CONSUMER);
            }
            while(highConsumerAddNumber-- > 0)
            {
                cellVector.push_back(cell::HIGH_CONSUMER);
            }
            if(cellVector.size())
            {
                int randomTimes=MapWidth*MapHeight;
                while(randomTimes--)
                {
                    int a=distribution(engine);
                    int b=distribution(engine);
                    std::swap(cellVector[a],cellVector[b]);
                }


                for(int i=0;i<MapWidth;++i)
                {
                    for(int j=0;j<MapHeight;++j)
                    {
                        if(Mainmap->cget(i, j).getType()==cell::NOTHING)
                        {
                            int index = cellVector.size() - 1;
                            int type=cellVector[index];
                            cellVector.pop_back();
                            Mainmap->cget(i, j).init(cell::LIVE,type);
                        }
                    }
                }
            }
        }

    }
    if(flag)
    {
        //SettingDialog->connect(settingdialog->yesButton,SIGNAL(clicked()),SettingDialog,SLOT(accept());
        threadRun->resume();
        threadRun->start();
    }
}

void MainWindow::damageFunction(){
    damage=true;
    bool flag=false;
    if(threadRun->isRunning())
    {
        threadRun->stop();
        flag=true;
    }
    unsigned int seed = (unsigned int)time(0) % 10000;
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<int> distribution(0,1);
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            if(distribution(engine))
            {
                if(Mainmap->cget(i,j).getState()==cell::LIVE) Mainmap->cget(i,j).setState(cell::DEAD);
                else if(Mainmap->cget(i,j).getState()==cell::DEAD) Mainmap->cget(i,j).init();
            }
        }
    }
    if(flag) threadRun->start();
}

void MainWindow::fineFunction(){
    /*
    double producer_f=0.7;
    double consumer_f=0.2;
    double highConsumer_f=0.1;
    ReStartFunction(producer_f, consumer_f, highConsumer_f);
    */
    bool flag=false;
    if(threadRun->isRunning())
    {
        threadRun->stop();
        flag=true;
    }
    for(int i=0;i<MapWidth;++i)
    {
        for(int j=0;j<MapHeight;++j)
        {
            if(Mainmap->cget(i, j).getState()==cell::LIVE)
            {
                Mainmap->cget(i, j).setStarvingTime(0);
            }
            else if(Mainmap->cget(i, j).getState()==cell::DEAD) Mainmap->cget(i, j).init();
            else if(Mainmap->cget(i, j).getState()==cell::EMPTY)
            {
                time_t seed = time(0) % 10000;
                std::uniform_int_distribution<int> FineDistribution(0,9);
                std::default_random_engine engine(seed);
                int burnRandom=FineDistribution(engine);
                std::cerr<<burnRandom<<std::endl;
                if(burnRandom>=0&&burnRandom<=5)
                {
                    Mainmap->cget(i, j).init(cell::LIVE,cell::PRODUCER);
                }
                else if(burnRandom>5&&burnRandom<=7)
                {
                    Mainmap->cget(i, j).init(cell::LIVE,cell::CONSUMER);
                }
                else
                {
                    Mainmap->cget(i, j).init(cell::LIVE,cell::HIGH_CONSUMER);
                }
            }
        }
    }
    if(flag) threadRun->start();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    painter = new QPainter;
    painter->begin(this);
    QImage image;
    if(damage)
    {
        image.load(":image/bg2.png");
        damage=false;
    }
    else image.load( ":image/bg.png" );

    painter->drawImage(0,0, image);

    QPixmap pixmap;
    for (int i = 0; i < MapWidth; ++i)
    {
        for (int j = 0; j < MapHeight; ++j)
        {

            painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH, WIDTH, pixmap);
            if (Mainmap->cget(i, j).getState() == cell::LIVE)
            {

                if (Mainmap->cget(i, j).getType() == cell::PRODUCER)
                {
                    QPixmap pixmap;
                    pixmap.load(":/image/glass.png");
                    painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.7, WIDTH * 0.7, pixmap);
                }
                else if (Mainmap->cget(i, j).getType() == cell::CONSUMER)
                {
                    QPixmap pixmap;
                    if(Mainmap->cget(i, j).getAge() < Mainmap->cget(i, j).getProduceAge())
                    {
                        pixmap.load(":image/mouse_young.png");
                        painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.7, WIDTH * 0.7, pixmap);
                    }
                    else
                    {
                        pixmap.load(":image/mouse.png");
                        painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.8, WIDTH * 0.8, pixmap);
                    }
                }
                else if (Mainmap->cget(i, j).getType() == cell::HIGH_CONSUMER)
                {
                    QPixmap pixmap;
                    if(Mainmap->cget(i, j).getAge() < Mainmap->cget(i, j).getProduceAge())
                    {
                        pixmap.load(":image/wolf_young.png");
                        painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH * 0.9, WIDTH * 0.9, pixmap);
                    }
                    else
                    {
                        pixmap.load(":image/wolf.png");
                        painter->drawPixmap(Ox + WIDTH * i, Oy + WIDTH * j, WIDTH, WIDTH, pixmap);
                    }
                }

            }
            else if (Mainmap->cget(i, j).getState() == cell::DEAD)
            {
                if (Mainmap->cget(i, j).getType() == cell::PRODUCER)
                {
                    QPixmap pixmap;
                    pixmap.load(":image/dead_leaf.png");
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
                    pixmap.load(":image/skull3.png");
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
