#ifndef _SHIFT_H_
#define _SHIFT_H_

#include "pin.h"

class Shift;

class Latch
{
public:
    Latch(Shift& shift, unsigned take_bits);

    Latch& operator<<(unsigned x);

    Latch& take_bits(unsigned take_bits) { take = take_bits; return *this; }
    
    ~Latch();
private:
    Shift& out;
    unsigned take;

    // wtf? 
    inline static void busy_sleep() {  } 
};

// Shift register
class Shift
{
public:
    #define UGLY (PinOut&&)
    Shift(PinOut&& dataa, PinOut&& latchh, PinOut&& clockk)
        : datap{UGLY dataa}, latchp{UGLY latchh}, clockp{UGLY clockk} {}
    #undef UGLY

    friend Latch;
    Latch latch(unsigned take_bits) { return Latch {*this, take_bits}; }

private:
    PinOut datap, latchp, clockp;
};

inline Latch::Latch(Shift& shift, unsigned take_bits)
    : out{shift}, take{take_bits} { out.latchp = 0; }

inline Latch::~Latch() { out.latchp = 1;/* busy_sleep();*/ }

#endif // _SHIFT_H_
