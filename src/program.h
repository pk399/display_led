#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <optional>
#include <array>

#include "config.h"
#include "picture.h"

using namespace pic;

class Input
{
private:
unsigned bits{};
public:
    Input() {}
    Input(unsigned bb): bits{bb} {}

    bool operator[](int idx) { return 1 & (bits >> idx); }
    void set(int idx) { bits |= 1 << idx; }
};

class Program
{
public:
virtual Program() = 0;

virtual std::optional<std::array<Pixel, ROWS*COLS>> update(unsgined delta_us, Input inputs) = 0;

virtual unsigned short preferredFPS()
{
    return DEFAULT_FPS;
}

virtual ~Program() = 0;
};


#endif // _PROGRAM_H_
