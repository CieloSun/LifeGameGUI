#include "thread.h"
#include<QMutexLocker>



Thread::Thread(cell::cellMap* _Mainmap):Mainmap(_Mainmap)
{
    stopped=false;
}

void Thread::setMap(cell::cellMap* _Mainmap)
{
    QMutexLocker locker(&mutex);
    Mainmap=_Mainmap;
}

void Thread::stop()
{
    QMutexLocker locker(&mutex);
    stopped=true;
}
void Thread::resume()
{
    QMutexLocker locker(&mutex);
    stopped=false;
}

int Thread::getNothingNumber() const
{
    return nothing_number;
}
int Thread::getProducerNumber() const
{
    return producer_number;
}

int Thread::getConsumerNumber() const
{
    return consumer_number;
}

int Thread::getHighConsumerNumber() const
{
    return high_consumer_number;
}

void Thread::run()
{
    forever
    {
        QMutexLocker locker(&mutex);
        if(stopped)
        {
            stopped=false;
            break;
        }
        bool have_living=false;
        for (int i = 0; i < Mainmap->getWidth(); ++i)
        {
            for (int j = 0; j < Mainmap->getHeight(); ++j)
            {
                if(Mainmap->cget(i,j).getType()!=cell::NOTHING||cell::DEAD)
                {
                    have_living=true;
                    goto start_game;//利用goto跳出多重循环
                }
            }
        }
        start_game:
        if(have_living==false)
        {
            stopped=true;
            emit End();
            break;
        }
        //每轮正式进行则初始化
        nothing_number=0;
        producer_number=0;
        consumer_number=0;
        high_consumer_number=0;
        for (int i = 0; i < Mainmap->getWidth(); ++i)
        {
            for (int j = 0; j < Mainmap->getHeight(); ++j)
            {
                if (Mainmap->cget(i,j).getType() == cell::NOTHING)
                {
                    nothing_number++;
                    Mainmap->burn(i, j);
                }
                else
                {
                    if (Mainmap->cget(i,j).getType() == cell::PRODUCER) producer_number++;
                    else if (Mainmap->cget(i,j).getType() == cell::CONSUMER) consumer_number++;
                    else if (Mainmap->cget(i,j).getType() == cell::HIGH_CONSUMER) high_consumer_number++;
                    Mainmap->exist(i, j);
                }
            }
        }
        emit ChangeScreen();
        Sleep(Mainmap->getSpeed());
    }
}

void Thread::restart(int _sp, double p_N, double c_N, double h_N)
{
    QMutexLocker locker(&mutex);
    Mainmap->loadMap(p_N,c_N,h_N);
    Mainmap->setSpeed(_sp);
    emit ChangeScreen();
}
