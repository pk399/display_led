#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "config.h"
#include "shift.h"

enum Pixel {RED = 1, GREEN = 2, BLUE = 4};

namespace display_impl
{

template <unsigned ROWS, unsigned COLS>
class Display
{
public:
    Display(Shift&& output)
        : out{(Shift&&) output} {}

    void draw(std::function<bool(Pixel, int x, int y)>);
    
    void draw(const bool* r, const bool* g, const bool* b)
    {
        draw([r, g, b](Pixel c, int x, int y) -> bool
        {
            const bool* p;
            switch (c)
            {
            case RED:
                p = r;
                break;
            case GREEN:
                p = g;
                break;
            case BLUE:
                p = b;
            }

            return p[COLS*y + x];
        });
    }

    void draw(const std::array<Pixel, ROWS*COLS> a)
    {
        draw([&a](Pixel col, int x, int y) -> bool 
            {       
                return a[x + y*COLS] | col;
            }
        );
    }

private:
    Shift out;
};

}

// Implementation
#include "display.cpp"

typedef display_impl::Display<ROWS, COLS> Display;

#endif // _DISPLAY_H_
