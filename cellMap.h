#pragma once
#include "cell.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <memory.h>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>

namespace cell
{
static const int MaxWidth = 2000;
static const int MaxHeight = 1000;
static const int DefaultWidth = 200;
static const int DefaultHeight = 100;

static const int FAST_SPEED = 100;
static const int NORMAL_SPEED = 500;
static const int SLOW_SPEED = 1000;

class cellMap
{
public:

    //Initilize the map
    cellMap(int = DefaultWidth, int = DefaultHeight);

    //Initilize the map with frequency
    //for seed<0 use timestamp as a seed
    void loadMap(double = 0.5, double = 0.1, double = 0.02, int seed = -1);

    //Count the number of target livings in its releam/environment, return a list of target
    //Return all neighbours if all=true
    std::vector<cell> count(int my_x, int my_y, int my_range, int ob_type, int ob_state, bool all);

    //make a new live on given position
    void burn(int x, int y);

    //check it they have predatory relation
    bool eat(cell& op1, cell& op2);

    //mark all objects unvisited,call it before each evolution
    void cleanVisitedState()
    {
        memset(visited,false,sizeof(visited));
    }

    //update given position
    //This is the main function of evolution
    void exist(int x, int y);

    //For CONSUMER and HIGH_CONSUMER, move its position
    void move(cell&op, int x, int y);

    //Get objects of given position
    cell &cget(int x, int y)
    {
        return array[x][y];
    }

    int getWidth()
    {
        return width;
    }
    int getHeight()
    {
        return height;
    }

    void setEvolution(double _evolution)
    {
        evolution = _evolution;
    }

    void setSpeed(int _speed)
    {
        speed = _speed;
    }

    int getSpeed() const
    {
        return speed;
    }

    std::default_random_engine manualeng; // random engine for manual operation
private:
    int width;
    int height;
    int speed;
    double evolution;
    cell array[MaxWidth][MaxHeight];
    bool visited[MaxWidth][MaxHeight];  //check if some objects are updated
    std::default_random_engine engine;  //random engine for evolution generator
};
}
