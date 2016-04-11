#include "thread.h"
#include<QMutexLocker>



Thread::Thread(cell::cellMap* _Mainmap): Mainmap(_Mainmap)
{
    stopped = false;
}

void Thread::setMap(cell::cellMap* _Mainmap)
{
    QMutexLocker locker(&mutex);
    Mainmap = _Mainmap;
}

void Thread::stop()
{
    QMutexLocker locker(&mutex);
    stopped = true;
}
void Thread::resume()
{
    QMutexLocker locker(&mutex);
    stopped = false;
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
        if (stopped)
        {
            stopped = false;
            break;
        }
        bool have_living = false;
        for (int i = 0; i < Mainmap->getWidth(); ++i)
        {
            for (int j = 0; j < Mainmap->getHeight(); ++j)
            {
                if (Mainmap->cget(i, j).getType() != cell::NOTHING && Mainmap->cget(i, j).getState() != cell::EMPTY)
                {
                    have_living = true;
                    goto start_game;
                }
            }
        }
start_game:
        if (have_living == false)
        {
            stopped = true;
            emit End();
            break;
        }
        Mainmap->cleanVisitedState();
        for (int i = 0; i < Mainmap->getWidth(); ++i)
        {
            for (int j = 0; j < Mainmap->getHeight(); ++j)
            {
                if (Mainmap->cget(i, j).getType() == cell::NOTHING || Mainmap->cget(i, j).getState() == cell::EMPTY)
                {
                    Mainmap->cget(i, j).init();
                    Mainmap->burn(i, j);
                }
                else
                {
                    Mainmap->exist(i, j);
                }
            }
        }
        //Init
        nothing_number = 0;
        producer_number = 0;
        consumer_number = 0;
        high_consumer_number = 0;
        for (int i = 0; i < Mainmap->getWidth(); ++i)
        {
            for (int j = 0; j < Mainmap->getHeight(); ++j)
            {
                if (Mainmap->cget(i, j).getType() == cell::NOTHING)
                {
                    nothing_number++;
                }
                else
                {
                    if (Mainmap->cget(i, j).getType() == cell::PRODUCER)
                    {
                        producer_number++;
                    }
                    else if (Mainmap->cget(i, j).getType() == cell::CONSUMER)
                    {
                        consumer_number++;
                    }
                    else if (Mainmap->cget(i, j).getType() == cell::HIGH_CONSUMER)
                    {
                        high_consumer_number++;
                    }
                }
            }
        }
        emit ChangeScreen();
        msleep(Mainmap->getSpeed());
    }
}

void Thread::restart(double p_N, double c_N, double h_N)
{
    QMutexLocker locker(&mutex);
    //Mainmap->setSpeed(_sp);
    Mainmap->loadMap(p_N, c_N, h_N);
    emit ChangeScreen();
}
