#include "common.h"
#include "shift.h"

auto Latch::operator<<(unsigned x) -> Latch&
{
    for (int i = 0; i < take; ++i)
    {
        out.clockp = 0;
        out.datap = 1 & (x >> i);
        out.clockp = 1;
        busy_sleep();
    }

    return *this;
}
