#ifndef _PROGRAMS_H_
#define _PROGRAMS_H_

#include "program.h"

#include "spiner.cpp"
#include "circles.cpp"
#include "carskiy_tetris.cpp"

Program* create_prog(int n)
{
    switch (n)
    {
        case 0: return new Circles;
        case 1: return new Tetris;
        default: return new Spiner;
    }
}

#endif // _PROGRAMS_H_
