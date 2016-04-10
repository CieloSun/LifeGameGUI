#include "cell.h"
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
        produceAge = PRODUCER_PRODUCE_AGE;
        starvingTimeLimit = PRODUCER_STARVING_TIME;
        break;
    case CONSUMER:
        liveNumber = CONSUMER_LN;
        deadNumber = CONSUMER_DN;
        range = CONSUMER_RANGE;
        ageLimit = CONSUMER_AGE;
        afterDeadLimit = CONSUMER_AFTER_DEAD;
        produceAge = CONSUMER_PRODUCE_AGE;
        starvingTimeLimit = CONSUMER_STARVING_TIME;
        break;
    case HIGH_CONSUMER:
        liveNumber = HIGH_CONSUMER_LN;
        deadNumber = HIGH_CONSUMER_DN;
        range = HIGH_CONSUMER_RANGE;
        ageLimit = HIGH_CONSUMER_AGE;
        afterDeadLimit = HIGH_CONSUMER_AFTER_DEAD;
        produceAge = HIGH_CONSUMER_PRODUCE_AGE;
        starvingTimeLimit = HIGH_CONSUMER_STARVING_TIME;
        break;
    case NOTHING:
        liveNumber = 0;
        deadNumber = 0;
        range = 0;
        ageLimit = 0;
        afterDeadLimit = 0;
        produceAge = 0;
        starvingTimeLimit = 0;
        break;
    default:
        break;
    }
    age = 0;
    afterDead = 0;
    starvingTime = 0;
}

void cell::cell::copy(cell const & ob)
{
    setState(ob.getState());
    setType(ob.getType());
    setRange(ob.getRange());
    setLiveNumber(ob.getLiveNumber());
    setDeadNumber(ob.getDeadNumber());
    setAgeLimit(ob.getAgeLimit());

    setAfterDeadLimit(ob.getAfterDeadLimit());
    setProduceAge(ob.getProduceAge());
    setStarvingTimeLimit(ob.getStarvingTimeLimit());
    setAge(0);
    setAfterDead(0);
    setStarvingTime(0);
}

