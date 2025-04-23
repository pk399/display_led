#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "shift.h"
#include <functional>

template <unsigned ROWS, unsigned COLS>
class Display
{
public:
    Display(Shift&& output)
        : out{(Shift&&) output} {}

    enum Color {Red = 0, Green = 1, Blue = 2};

    void draw(std::function<bool(Color, int x, int y)> f);
    
    void draw(const bool* r, const bool* g, const bool* b)
    {
        draw([r, g, b](Color c, int x, int y) -> bool
        {
            const bool* p;
            switch (c)
            {
            case Red:
                p = r;
                break;
            case Green:
                p = g;
                break;
            case Blue:
                p = b;
            }

            return p[COLS*y + x];
        });
    }

private:
    Shift out;
};

// Implementation
#include "display.cpp"

#endif // _DISPLAY_H_
