#ifndef THREAD_H
#define THREAD_H
#include "cellMap.h"
#include "windows.h"

#include<QMutex>
#include<QThread>
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(cell::cellMap* _Mainmap);
    void setMap(cell::cellMap* _Mainmap);

    void stop();
    void resume();
    void restart(double p_N, double c_N, double h_N);
    int getNothingNumber() const;
    int getProducerNumber() const;
    int getConsumerNumber() const;
    int getHighConsumerNumber() const;

    ~Thread() = default;

signals:
    void ChangeScreen();

    void End();
protected:
    void run();


private:
    volatile int nothing_number;
    volatile int producer_number;
    volatile int consumer_number;
    volatile int high_consumer_number;
    volatile bool stopped;
    cell::cellMap* Mainmap;
    QMutex mutex;
};

#endif // THREAD_H
