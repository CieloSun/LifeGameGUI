#include "cellMap.h"
#include "iostream"//调试用

//构造函数
cell::cellMap::cellMap(int _width, int _height)
	:width(_width), height(_height)
{
	
	for (int i = 0;i < width;++i)
	{
		for (int j = 0;j < height;++j)
		{
			array[i][j].init();
			
		}
	}
	
			
	producerAmount = 0;
	consumerAmount = 0;
	highConsumerAmount = 0;
	pause = false;
}

//设置完毕，生成初始态
void cell::cellMap::loadMap(double producer_freq, double consumer_freq, double highConsumer_freq)
{
	double sum_freq = highConsumer_freq + consumer_freq + producer_freq;
	if (producer_freq < 0 || consumer_freq < 0 || highConsumer_freq < 0 || sum_freq > 1) return;

	time_t seed = time(0);
	std::default_random_engine engine(seed);
	std::uniform_real_distribution<double> distribution(0, 1);

	for (int i = 0;i < width;++i)
	{
		for (int j = 0;j < height;++j)
		{
			//std::cout << "test" << std::endl;
			double freq = distribution(engine);
			//std::cout << freq << std::endl;随机数没问题
			if (freq < highConsumer_freq) array[i][j].init(cell::LIVE, cell::HIGH_CONSUMER);
			else if (freq < highConsumer_freq + consumer_freq) array[i][j].init(cell::LIVE, cell::CONSUMER);
			else if (freq < sum_freq) array[i][j].init(cell::LIVE, cell::PRODUCER);
			else array[i][j].init();
		}
	}
}
//查询周围目标数量
std::vector<cell::cell> cell::cellMap::count(int my_x, int my_y, int my_range, int ob_type, int ob_state)
{
	std::vector<cell> countVector;
	countVector.clear();
	for (int i = my_x - my_range;i <= my_x + my_range;++i)
	{
		for (int j = my_y - my_range;j <= my_y + my_range;++j)
		{
			if (0 <= i&&i < width && 0 <= j&&j <= height)
			{
				if (array[i][j].getType() == ob_type || array[i][j].getState() == ob_state)
				{
					countVector.push_back(array[i][j]);
				}
			}
		}
	}
	return countVector;
}
//判断出生,传入一个空位置
bool cell::cellMap::burn(int x, int y)
{
	//新建随机种子
	time_t seed = time(0);
	//先检查高级消费者
	std::vector<cell> countVector = count(x, y, cell::HIGH_CONSUMER_RANGE, cell::HIGH_CONSUMER, cell::LIVE);
	if (countVector.size() < cell::HIGH_CONSUMER_LN)
	{
		countVector.clear();
		//再检查初级消费者
		countVector = count(x, y, cell::CONSUMER_RANGE, cell::CONSUMER, cell::LIVE);
		if (countVector.size() < cell::CONSUMER_LN)
		{
			countVector.clear();
			//最后检查生产者
			countVector = count(x, y, cell::PRODUCER_RANGE, cell::PRODUCER, cell::LIVE);
			if (countVector.size() < cell::PRODUCER_LN)
			{
				return false;
			}
		}
	}
	std::uniform_int_distribution<int> distribution(0, countVector.size());
	std::default_random_engine engine(seed);
	int motherIndex = distribution(engine);
	//无性繁殖
	array[x][y].copy(countVector[motherIndex]);
	return true;
}
//检查是否有捕食关系,第一个参数是捕食者，默认为LIVE状态
bool cell::cellMap::eat(cell& op1, cell& op2)
{
	if (op1.getType() != cell::PRODUCER)
	{
		if (op2.getState() == cell::LIVE)
		{
			if ((op1.getType() == cell::HIGH_CONSUMER&&op2.getType() == cell::CONSUMER)
				|| (op1.getType() == cell::CONSUMER&&op2.getType() == cell::PRODUCER))
			{
				return true;
			}
		}
		else if (op2.getState() == cell::DEAD)
		{
			if (!(op1.getType() == cell::HIGH_CONSUMER&&op2.getType() == cell::PRODUCER))
			{
				return true;
			}
		}
	}
	return false;
}
//检查存在个体的格子
void cell::cellMap::exist(int x, int y)
{
	//自然死亡
	if (array[x][y].getState() == cell::LIVE)
	{
		if (array[x][y].getAge() >= array[x][y].getAgeLimit())
		{
			array[x][y].setState(cell::DEAD);
		}
		else
		{
			//年龄增长
			array[x][y].setAge(array[x][y].getAge() + 1);
			//判断捕食
			if (array[x][y].getType() != cell::PRODUCER)
			{
				bool full = false;
				for (int i = x - array[x][y].getRange();i <= x + array[x][y].getRange();++i)
				{
					for (int j = y - array[x][y].getRange();j <= y + array[x][y].getRange();++j)
					{
						if (eat(array[x][y], array[i][j]))
						{
							array[x][y].init();
							full = true;
						}
					}
				}
				if (!full) array[x][y].setState(cell::DEAD);
			}
			//判断竞争
			if ((count(x, y, array[x][y].getRange(), array[x][y].getType(), cell::LIVE).size()) >= array[x][y].getDeadNumber())
			{
				array[x][y].setState(cell::DEAD);
			}
		}
	}
	else if (array[x][y].getState() == cell::DEAD)
	{
		if (array[x][y].getAfterDead() >= array[x][y].getAfterDeadLimit())
		{
			array[x][y].init();
		}
		else
		{
			array[x][y].setAfterDead(array[x][y].getAfterDead() + 1);
		}
	}	 
}

void cell::cellMap::runMap(cellMap *ob)
{
    while (!ob->pause)
    {
        for (int i = 0;i < ob->width;++i)
        {
            for (int j = 0;j < ob->height;++j)
            {
                if (ob->array[i][j].getType() == cell::NOTHING)
                {
                    ob->burn(i, j);
                }
                else
                {
                    ob->exist(i, j);
                }
            }
        }
        Sleep(500);
        //ob.outputMap(std::cout);
    }

}

void cell::cellMap::startMap(cellMap *ob)
{
    std::thread t(runMap, ob);
    t.detach();
}

void cell::cellMap::pauseMap(cellMap *ob)
{
    ob->pause = true;
}

void cell::cellMap::resumeMap(cellMap *ob)
{
    ob->pause = false;
    startMap(ob);
}


void cell::cellMap::outputMap(std::ostream &os)
{
	if (os)
	{
		os << width << " " << height << std::endl;
		for (int i = 0;i < width;++i)
		{
			for (int j = 0;j < height;++j)
			{
				os << array[i][j].getType() << " " << array[i][j].getState() << "  ";
			}
			os << std::endl;
		}
	}
}
