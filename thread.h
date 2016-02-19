#ifndef THREAD_H
#define THREAD_H
#include "cellMap.h"
#include "windows.h"

#include<QThread>
class Thread :public QThread
{
    Q_OBJECT
public:
    Thread(cell::cellMap* _Mainmap):Mainmap(_Mainmap)
    {
        stopped=false;
    }
    Thread()
    {
        stopped=false;
    }

    Thread(Thread const&)=delete;
    Thread& operator=(Thread const&)=delete;

    void setMap(cell::cellMap* _Mainmap)
    {
        Mainmap=_Mainmap;
    }

    void stop()
    {
        stopped=true;
    }


protected:
    void run()
    {
        while (!stopped)
        {
            for (int i = 0; i < Mainmap->getWidth(); ++i)
            {
                for (int j = 0; j < Mainmap->getHeight(); ++j)
                {
                    if (Mainmap->cget(i,j).getType() == cell::cell::NOTHING)
                    {
                        Mainmap->burn(i, j);
                    }
                    else
                    {
                        Mainmap->exist(i, j);
                    }
                }
            }
            Sleep(500);
        }
    }


private:
    volatile bool stopped;
    cell::cellMap* Mainmap;
};

#endif // THREAD_H
