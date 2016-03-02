#include "cellMap.h"
#include "iostream"//调试用


//构造函数
cell::cellMap::cellMap(int _width, int _height)
    : width(_width), height(_height)
{

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            cget(i, j).init(EMPTY, NOTHING);
        }
    }
    //默认突变概率0.1
    evolution = 0.1;
    speed = NORMAL_SPEED;
}

//设置完毕，生成初始态
void cell::cellMap::loadMap(double producer_freq, double consumer_freq, double highConsumer_freq)
{
    double sum_freq = highConsumer_freq + consumer_freq + producer_freq;
    if (producer_freq < 0 || consumer_freq < 0 || highConsumer_freq < 0 || sum_freq > 1)
    {
        return;
    }

    time_t seed = time(0);
    std::default_random_engine engine(seed);
    std::uniform_real_distribution<double> distribution(0, 1);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            double freq = distribution(engine);
            if (freq < highConsumer_freq)
            {
                cget(i, j).init(LIVE, HIGH_CONSUMER);
            }
            else if (freq < highConsumer_freq + consumer_freq)
            {
                cget(i, j).init(LIVE, CONSUMER);
            }
            else if (freq < sum_freq)
            {
                cget(i, j).init(LIVE, PRODUCER);
            }
            else
            {
                cget(i, j).init(EMPTY, NOTHING);
            }
        }
    }
}
//查询周围目标数量
std::vector<cell::cell> cell::cellMap::count(int my_x, int my_y, int my_range, int ob_type, int ob_state)
{
    std::vector<cell> countVector;
    countVector.clear();
    for (int i = my_x - my_range; i <= my_x + my_range; ++i)
    {
        for (int j = my_y - my_range; j <= my_y + my_range; ++j)
        {
            if (0 <= i && i < width && 0 <= j && j <= height)
            {
                if ((cget(i, j).getType() == ob_type || cget(i, j).getState() == ob_state)
                        && (i != my_x && j != my_y))
                    //修改后不再计算自己，防止出现奇怪的错误
                {
                    countVector.push_back(array[i][j]);
                }
            }
        }
    }
    return countVector;
}
//判断出生,传入一个空位置
void cell::cellMap::burn(int x, int y)
{
    if (cget(x, y).getType() == NOTHING && cget(x, y).getState() == EMPTY)
    {
        //新建随机种子
        time_t seed = time(0);
        //先检查高级消费者
        std::vector<cell> countVector = count(x, y, HIGH_CONSUMER_RANGE, HIGH_CONSUMER, LIVE);
        if (countVector.size() < HIGH_CONSUMER_LN)
        {
            countVector.clear();
            //再检查初级消费者
            countVector = count(x, y, CONSUMER_RANGE, CONSUMER, LIVE);
            if (countVector.size() < CONSUMER_LN)
            {
                countVector.clear();
                //最后检查生产者
                countVector = count(x, y, PRODUCER_RANGE, PRODUCER, LIVE);
                if (countVector.size() < PRODUCER_LN)
                {
                    countVector.clear();
                    return;
                }
            }
        }
        std::uniform_int_distribution<int> distribution(0, countVector.size());
        std::default_random_engine engine(seed);
        int motherIndex = distribution(engine);
        //无性繁殖
        cget(x, y).copy(countVector[motherIndex]);

        //有一定概率进行突变
        std::uniform_real_distribution<double> distribution2(0, 1);
        std::default_random_engine engine2(seed);
        if (distribution2(engine2) < evolution)
        {
            //在deadNumber,range,ageLimit,afterDeadLimit中选择一个突变
            std::uniform_int_distribution<int> distribution3(0, 7);
            std::default_random_engine engine3(seed);
            switch (distribution3(engine3))
            {
            case 0:
                cget(x, y).setDeadNumber(cget(x, y).getDeadNumber() + 1);
                break;
            case 1:
                cget(x, y).setDeadNumber(cget(x, y).getDeadNumber() - 1);
                break;
            case 2:
                cget(x, y).setRange(cget(x, y).getRange() + 1);
                break;
            case 3:
                cget(x, y).setRange(cget(x, y).getRange() - 1);
                break;
            case 4:
                cget(x, y).setAgeLimit(cget(x, y).getAgeLimit() + 1);
                break;
            case 5:
                cget(x, y).setAgeLimit(cget(x, y).getAgeLimit() - 1);
                break;
            case 6:
                cget(x, y).setAfterDeadLimit(cget(x, y).getAfterDeadLimit() + 1);
                break;
            case 7:
                cget(x, y).setAfterDeadLimit(cget(x, y).getAfterDeadLimit() - 1);
                break;
            default:
                break;
            }
        }
    }
}
//检查是否有捕食关系,第一个参数是捕食者，默认为LIVE状态
bool cell::cellMap::eat(cell& op1, cell& op2)
{
    if (op1.getState() == LIVE)
    {
        if (op1.getType() != PRODUCER)
        {
            if (op2.getState() == LIVE)
            {
                if ((op1.getType() == HIGH_CONSUMER && op2.getType() == CONSUMER)
                        || (op1.getType() == CONSUMER && op2.getType() == PRODUCER))
                {
                    return true;
                }
            }
            else if (op2.getState() == DEAD)
            {
                if (!(op1.getType() == HIGH_CONSUMER && op2.getType() == PRODUCER))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
//检查存在个体的格子
void cell::cellMap::exist(int x, int y)
{
    if (cget(x, y).getState() != EMPTY && cget(x, y).getType() != NOTHING)
    {
        if (cget(x, y).getState() == DEAD)
        {
            if (cget(x, y).getAfterDead() >= cget(x, y).getAfterDeadLimit())
            {
                cget(x, y).init(EMPTY, NOTHING);
            }
            else
            {
                cget(x, y).setAfterDead(cget(x, y).getAfterDead() + 1);
            }
        }

        else if (cget(x, y).getState() == LIVE)
        {
            //自然死亡
            if (cget(x, y).getAge() >= cget(x, y).getAgeLimit())
            {
                cget(x, y).setState(DEAD);
            }
            else
            {
                //年龄增长
                cget(x, y).setAge(cget(x, y).getAge() + 1);
                //判断捕食
                if (cget(x, y).getType() != PRODUCER)
                {
                    bool full = false;
                    for (int i = x - cget(x, y).getRange(); i <= x + cget(x, y).getRange(); ++i)
                    {
                        for (int j = y - cget(x, y).getRange(); j <= y + cget(x, y).getRange(); ++j)
                        {
                            if (0 <= i && i < width && 0 <= j && j <= height)
                            {
                                if (eat(cget(x, y), cget(i, j)) && x != i && y != j)
                                {
                                    cget(x, y).init(EMPTY, NOTHING);
                                    full = true;
                                }
                            }
                        }
                    }
                    if (!full)
                    {
                        cget(x, y).setState(DEAD);
                    }
                }
                //判断竞争
                if ((count(x, y, cget(x, y).getRange(), cget(x, y).getType(), LIVE).size()) >= cget(x, y).getDeadNumber())
                {
                    cget(x, y).setState(DEAD);
                }
            }
        }
    }
}
