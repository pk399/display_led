#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "common.h"
#include "config.h"
#include "display.h"

typedef std::vector<int> Input;

class Program
{
public:
virtual std::pair<bool, std::array<Pixel, ROWS*COLS>> update(unsigned delta_us, Input& inputs) = 0;

virtual unsigned short preferredFPS()
{
    return DEFAULT_FPS;
}

};


#endif // _PROGRAM_H_
