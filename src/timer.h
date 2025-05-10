#ifndef _TIMER_H_
#define _TIMER_H_

#include "common.h"

namespace DONOTTOUCH
{
extern "C" int64_t alarm_callback(alarm_id_t id, void* user_data); // pstd
}

class Timer
{
private:
bool rrunning {true};
bool (*callback)();
unsigned delay;
alarm_pool_t* pool; // pstd
alarm_id_t alarm_id; // pstd

public:
Timer(unsigned delay_us, bool(*callbackk)()): delay{delay_us}, callback{callbackk}
{
    pool = pstd::alarm_pool_get_default();

    alarm_id = pstd::alarm_pool_add_alarm_in_us(pool, delay, &DONOTTOUCH::alarm_callback, this, false);
}

bool running() { return rrunning; }

~Timer()
{
    pstd::alarm_pool_cancel_alarm(pool, alarm_id);
}

friend pstd::int64_t DONOTTOUCH::alarm_callback(pstd::alarm_id_t id, void* user_data);
};

namespace DONOTTOUCH
{
extern "C" pstd::int64_t alarm_callback(pstd::alarm_id_t id, void* user_data)
{
    Timer* that = reinterpret_cast<Timer*>(user_data);

    bool res = that->callback();
    that->rrunning = res;

    return res ? that->delay : 0;
}
}

#endif // _TIMER_H_
