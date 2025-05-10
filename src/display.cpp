#include "common.h"

namespace display_impl
{
template <unsigned ROWS, unsigned COLS>
void Display<ROWS, COLS>::draw(std::function<bool(Pixel c, int x, int y)> f)
{
    // !!! Shift registers are loaded in reverse order

    for (int i = 0; i < ROWS; ++i)
    {
        auto l = out.latch( 1 );

        for (int j = (ROWS - 1); j >= 0; --j)
        {
            l << ( i == j );
        }

        for (int j = (COLS - 1); j >= 0; --j)
        {
            l << f(RED, j, i) << f(GREEN, j, i) << f(BLUE, j, i);
        }
    }

    // Leave display turned off
    auto l = out.latch( 1 );
    for (int i = 0; i < COLS*3 + ROWS; i++)
    {
        l << 0;
    }
}

}
