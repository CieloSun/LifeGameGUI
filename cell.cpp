﻿#include "cell.h"
cell::cell::cell(int _state, int _type)
{
    init(_state, _type);
}

void cell::cell::init(int _state, int _type)
{
    state = _state;
    type = _type;
    switch (type)
    {
    case PRODUCER:
        liveNumber = PRODUCER_LN;
        deadNumber = PRODUCER_DN;
        range = PRODUCER_RANGE;
        ageLimit = PRODUCER_AGE;
        afterDeadLimit = PRODUCER_AFTER_DEAD;
        produceAge=PRODUCER_PRODUCE_AGE;
        break;
    case CONSUMER:
        liveNumber = CONSUMER_LN;
        deadNumber = CONSUMER_DN;
        range = CONSUMER_RANGE;
        ageLimit = CONSUMER_AGE;
        afterDeadLimit = CONSUMER_AFTER_DEAD;
        produceAge=CONSUMER_PRODUCE_AGE;
        break;
    case HIGH_CONSUMER:
        liveNumber = HIGH_CONSUMER_LN;
        deadNumber = HIGH_CONSUMER_DN;
        range = HIGH_CONSUMER_RANGE;
        ageLimit = HIGH_CONSUMER_AGE;
        afterDeadLimit = HIGH_CONSUMER_AFTER_DEAD;
        produceAge=HIGH_CONSUMER_PRODUCE_AGE;
        break;
    case NOTHING:
        liveNumber = 0;
        deadNumber = 0;
        range = 0;
        ageLimit = 0;
        afterDeadLimit = 0;
        produceAge=0;
        break;
    default:
        break;
    }
    age = 0;
    afterDead = 0;
}

void cell::cell::copy(cell const & ob)
{
    state = ob.state;
    type = ob.type;
    range = ob.range;
    liveNumber = ob.liveNumber;
    deadNumber = ob.deadNumber;
    ageLimit = ob.ageLimit;
    afterDeadLimit = ob.afterDeadLimit;
    produceAge=ob.produceAge;
    //复制即是新生
    age = 0;
    afterDead = 0;
}

