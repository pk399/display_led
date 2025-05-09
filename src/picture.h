#ifndef _PICTURE_H_
#define _PICTURE_H_

#include <array>

#include <display.h>
#include <config.h>

namespace pic
{
    enum Pixel
    {
        RED = 1;
        GREEN = 2;
        BLUE = 4;
    };

    class DrawArray
    {
    private:
    std::array<Pixel, ROWS*COLS>& arr;
    public:
    DrawArray(std::array<Pixel, ROWS*COLS>& a): arr{a} {}
    
    bool operator()(Color c, int x, int y)
    {
        Pixel col;
        switch (c)
        {
            case Red: col = RED;
            case Green: col = GREEN;
            case Blue: col = BLUE;
            default: col = 0;
        }

        return arr[x + y*COLS] | col;
    }
    }; 
}

#endif // _PICTURE_H_
