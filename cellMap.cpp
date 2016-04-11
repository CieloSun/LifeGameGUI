#include "cellMap.h"
#include "iostream" //For Debug
#include <QTime>

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
    //Initial possibility of mutation
    evolution = 0.1;
    speed = NORMAL_SPEED;
    manualeng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    loadMap();
}

void cell::cellMap::loadMap(double producer_freq, double consumer_freq, double highConsumer_freq,  int seed)
{
    double sum_freq = highConsumer_freq + consumer_freq + producer_freq;
    if (producer_freq < 0 || consumer_freq < 0 || highConsumer_freq < 0 || sum_freq > 1)
    {
        return;
    }

    engine.seed((seed<0)? std::chrono::system_clock::now().time_since_epoch().count() : seed);
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

std::vector<cell::cell> cell::cellMap::count(int my_x, int my_y, int my_range, int ob_type, int ob_state, bool all=false)
{
    std::vector<cell> countVector;
    countVector.clear();
    for (int i = my_x - my_range; i <= my_x + my_range; ++i)
    {
        for (int j = my_y - my_range; j <= my_y + my_range; ++j)
        {
            if (0 <= i && i < width && 0 <= j && j < height)
            {
                if ((cget(i, j).getType() == ob_type && cget(i, j).getState() == ob_state)
                        && (i != my_x && j != my_y))
                    //Don't count itself
                {
                    if(all)
                    {
                        countVector.push_back(array[i][j]);
                    }
                    //check if fertile
                    else if (cget(i, j).getProduceAge() <= cget(i, j).getAge())
                    {
                        countVector.push_back(array[i][j]);
                    }

                }
            }
        }
    }
    return countVector;
}

void cell::cellMap::burn(int x, int y)
{
    if (cget(x, y).getState() == NOTHING)
    {
        //count HIGH_CONSUMER
        std::vector<cell> countVector = count(x, y, HIGH_CONSUMER_RANGE, HIGH_CONSUMER, LIVE);
        if (countVector.size() < HIGH_CONSUMER_LN)
        {
            countVector.clear();
            //count CONSUMER
            countVector = count(x, y, CONSUMER_RANGE, CONSUMER, LIVE);
            if (countVector.size() < CONSUMER_LN)
            {
                countVector.clear();
                //count PRODUCER
                countVector = count(x, y, PRODUCER_RANGE, PRODUCER, LIVE);

                if (countVector.size() < PRODUCER_LN)
                {
                    countVector.clear();
                    return;
                }
            }
        }

        std::uniform_int_distribution<int> distribution(0, countVector.size());

        int motherIndex = distribution(engine);
        //vegetative propagation
        cget(x, y).copy(countVector[motherIndex]);

        std::uniform_real_distribution<double> distribution2(0, 1);
        std::uniform_int_distribution<int> distribution3(0, 5);
        std::uniform_int_distribution<int> distribution4(0, 4);
        std::uniform_int_distribution<int> distribution5(1, 4);
        std::uniform_int_distribution<int> distribution6(0, 100);
        std::uniform_int_distribution<int> distribution7(0, 10);
        std::uniform_int_distribution<int> distribution8(0, 30);
        std::uniform_int_distribution<int> distribution9(10, 50);
        if (distribution2(engine) < evolution)
            //possibility of mutation
        {
            //one of these attributes will be variable

            switch (distribution3(engine))
            {
            case 0:
                cget(x, y).setDeadNumber(distribution4(engine));
                break;
            case 1:
                cget(x, y).setRange(distribution5(engine));
                break;
            case 2:
                cget(x, y).setAgeLimit(distribution6(engine));
                break;
            case 3:
                cget(x, y).setAfterDeadLimit(distribution7(engine));
                break;
            case 4:
                cget(x, y).setStarvingTimeLimit(distribution8(engine));
                break;
            case 5:
                cget(x, y).setProduceAge(distribution9(engine));
                break;
            default:
                break;
            }

        }

    }

}

bool cell::cellMap::eat(cell& op1, cell& op2)
{
    //if(op2.getState() == DEAD) return true;
    if ((op1.getType() == HIGH_CONSUMER && op2.getType() == CONSUMER)
            || (op1.getType() == CONSUMER && op2.getType() == PRODUCER))
    {
        return true;
    }
    return false;
}

void cell::cellMap::move(cell& op, int x,int y)
{
    cget(x,y).copy(op);
    cget(x,y).setAge(op.getAge());
    cget(x,y).setStarvingTime(op.getStarvingTime());
    op.init();
}

void cell::cellMap::exist(int x, int y)
{
    if (visited[x][y]) return;
    if (cget(x, y).getState() == DEAD)
    {
        if (cget(x, y).getAfterDead() >= cget(x, y).getAfterDeadLimit())
        {
            cget(x, y).init();
        }
        else
        {
            cget(x, y).setAfterDead(cget(x, y).getAfterDead() + 1);
        }
    }

    else if (cget(x, y).getState() == LIVE)
    {
        //Reach its age limit
        if (cget(x, y).getAge() >= cget(x, y).getAgeLimit())
        {
            cget(x, y).setState(DEAD);
        }
        else
        {
            //Grown age
            cget(x, y).setAge(cget(x, y).getAge() + 1);
            //find all prey
            if (cget(x, y).getType() != PRODUCER)
            {
                int full = 0;
                for (int i = x - cget(x, y).getRange(); i <= x + cget(x, y).getRange(); ++i)
                {
                    for (int j = y - cget(x, y).getRange(); j <= y + cget(x, y).getRange(); ++j)
                    {
                        if (0 <= i && i < width && 0 <= j && j < height)
                        {
                            if (eat(cget(x, y), cget(i, j)))
                            {
                                cget(i, j).init();
                                ++full;
                            }
                        }
                    }
                }
                if (full > 0)
                {
                    //Addition starvation for no prey
                    int temp = cget(x, y).getStarvingTime() - full;
                    cget(x, y).setStarvingTime((temp>=0)?temp:0);
                    visited[x][y] = true;
                }
                else
                {
                    cget(x, y).setStarvingTime(cget(x, y).getStarvingTime() + 1);
                    int my_range=cget(x,y).getRange();
                    std::vector<std::pair<int,int> > target;
                    for (int i = x + my_range; i >=x - my_range; --i)
                    {
                        for (int j = y + my_range; j >= y - my_range; --j)
                        {
                            if (0 <= i && i < width && 0 <= j && j < height)
                            {

                                if(cget(x,y).getType()==CONSUMER)
                                {
                                    if ((cget(i, j).getType() == NOTHING)&& (i != x && j != y))
                                    {
                                        target.push_back(std::make_pair(i,j));
                                    }
                                }
                                else if(cget(x, y).getType()==HIGH_CONSUMER)
                                {
                                    if (cget(i, j).getType() == NOTHING || cget(i, j).getType() == PRODUCER || cget(i, j).getType() == CONSUMER)
                                    {
                                        target.push_back(std::make_pair(i,j));
                                    }
                                }
                            }
                        }
                    }
                    if (!target.empty())
                    {
                        std::uniform_int_distribution<> dist(0,target.size() - 1);
                        auto it = target.begin() + dist(engine);
                        move(cget(x,y),it->first,it->second);
                        x = it->first; y = it->second;
                    }

                    visited[x][y] = true;
                }
                if(cget(x, y).getStarvingTime() > cget(x, y).getStarvingTimeLimit()) cget(x, y).setState(DEAD);
                //check if competition is happened
                if ((count(x, y, cget(x, y).getRange(), cget(x, y).getType(), LIVE, true).size()) >= cget(x, y).getDeadNumber())
                {
                    cget(x, y).setState(DEAD);
                }
            }
        }
    }
}
