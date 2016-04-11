#pragma once
#include<iostream>
namespace cell
{

//物种三个状态
static const int EMPTY = 0;
static const int LIVE = 1;
static const int DEAD = 2;
//三种物种
static const int NOTHING = 0;
static const int PRODUCER = 1;
static const int CONSUMER = 2;
static const int HIGH_CONSUMER = 3;
//生产者参数
static const int PRODUCER_LN = 1;
static const int PRODUCER_DN = 7;
static const int PRODUCER_RANGE = 1;
static const int PRODUCER_AGE = 30;
static const int PRODUCER_AFTER_DEAD = 10;
static const int PRODUCER_PRODUCE_AGE = 0;
static const int PRODUCER_STARVING_TIME = 99;
//消费者参数
static const int CONSUMER_LN = 1;
static const int CONSUMER_DN = 3;
static const int CONSUMER_RANGE = 1;
static const int CONSUMER_AGE = 50;
static const int CONSUMER_AFTER_DEAD = 5;
static const int CONSUMER_PRODUCE_AGE = 20;
static const int CONSUMER_STARVING_TIME= 8;
//高级消费者参数
static const int HIGH_CONSUMER_LN = 2;
static const int HIGH_CONSUMER_DN = 3;
static const int HIGH_CONSUMER_RANGE = 2;
//static const int HIGH_CONSUMER_SEARCH_RANGE = 4;
static const int HIGH_CONSUMER_AGE = 100;
static const int HIGH_CONSUMER_AFTER_DEAD = 20;
static const int HIGH_CONSUMER_PRODUCE_AGE = 30;
static const int HIGH_CONSUMER_STARVING_TIME = 20;
class cell
{
public:
    cell(int _state = EMPTY, int _type = NOTHING);
    void init(int _state = EMPTY, int _type = NOTHING);
    cell(cell const&) = default;
    cell& operator=(cell const&) = delete;
    void copy(cell const&);
    //  所有变量都提供了set函数和get函数
    void setState(int _state)
    {
        state = _state;
    }
    int getState() const
    {
        return state;
    }

    void setType(int _type)
    {
        type = _type;
    }
    int getType()
    {
        //TODO: Force overwrite errors
        if (type < NOTHING || type > HIGH_CONSUMER) init();
        return type;
    }

    void setLiveNumber(int _liveNumber)
    {
        liveNumber = _liveNumber;
    }
    int getLiveNumber() const
    {
        return liveNumber;
    }

    void setDeadNumber(int _deadNumber)
    {
        deadNumber = _deadNumber;
    }

    int getDeadNumber() const
    {
        return deadNumber;
    }

    void setRange(int _range)
    {
        range = _range;
    }
    int getRange() const
    {
        return range;
    }

    void setAgeLimit(int _ageLimit)
    {
        ageLimit = _ageLimit;
    }
    int getAgeLimit() const
    {
        return ageLimit;
    }

    void setAfterDeadLimit(int _afterDeadLimit)
    {
        afterDeadLimit = _afterDeadLimit;
    }
    int getAfterDeadLimit() const
    {
        return afterDeadLimit;
    }

    void setAge(int _age)
    {
        age = _age;
    }
    int getAge() const
    {
        return age;
    }

    void setAfterDead(int _afterDead)
    {
        afterDead = _afterDead;
    }
    int getAfterDead() const
    {
        return afterDead;
    }

    void setProduceAge(int _produceAge)
    {
        produceAge = _produceAge;
    }
    int getProduceAge() const
    {
        return produceAge;
    }

    void setStarvingTime(int _starvingTime)
    {
        starvingTime = _starvingTime;
    }
    int getStarvingTime() const
    {
        return starvingTime;
    }

    void setStarvingTimeLimit(int _starvingTimeLimit)
    {
        starvingTimeLimit = _starvingTimeLimit;
    }
    int getStarvingTimeLimit() const
    {
        return starvingTimeLimit;
    }

private:
    //状态
    int state;
    //种类
    int type;
    //出生需求数
    int liveNumber;
    //生存极限数
    int deadNumber;
    //生存辐射范围
    int range;
    //寿命限制
    int ageLimit;
    //腐烂时间限制
    int afterDeadLimit;
    //年龄
    int age;
    //腐烂时间
    int afterDead;
    //可育年龄
    int produceAge;
    //饥饿时间
    int starvingTime;
    //饥饿时限
    int starvingTimeLimit;

};
}
