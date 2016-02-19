#pragma once
#include<iostream>
namespace cell
{
class cell
{
public:
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
    static const int PRODUCER_DN = 6;
    static const int PRODUCER_RANGE = 1;
    static const int PRODUCER_AGE = 5;
    static const int PRODUCER_AFTER_DEAD = 1;
    //消费者参数
    static const int CONSUMER_LN = 2;
    static const int CONSUMER_DN = 5;
    static const int CONSUMER_RANGE = 2;
    static const int CONSUMER_AGE = 20;
    static const int CONSUMER_AFTER_DEAD = 2;
    //高级消费者参数
    static const int HIGH_CONSUMER_LN = 3;
    static const int HIGH_CONSUMER_DN = 4;
    static const int HIGH_CONSUMER_RANGE = 3;
    static const int HIGH_CONSUMER_AGE = 50;
    static const int HIGH_CONSUMER_AFTER_DEAD = 3;


    cell(int _state = EMPTY, int _type = NOTHING);
    void init(int _state = EMPTY, int _type = NOTHING);
    cell(cell const&) = default;
    cell& operator=(cell const&) = delete;
    void copy(cell const&);

    void setState(int _state)
    {
        state = _state;
    }
    int getState()
    {
        return state;
    }
    int getType()
    {
        return type;
    }
    int getLiveNumber()
    {
        return liveNumber;
    }
    int getDeadNumber()
    {
        return deadNumber;
    }
    int getRange()
    {
        return range;
    }
    int getAgeLimit()
    {
        return ageLimit;
    }
    int getAfterDeadLimit()
    {
        return afterDeadLimit;
    }
    void setAge(int _age)
    {
        age = _age;
    }
    int getAge()
    {
        return age;
    }
    void setAfterDead(int _afterDead)
    {
        afterDead = _afterDead;
    }
    int getAfterDead()
    {
        return afterDead;
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

};
}
