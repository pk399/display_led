template <unsigned ROWS, unsigned COLS>
void Display<ROWS, COLS>::draw(bool (*f)(Color, int x, int y))
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
            l << f(Red, j, i) << f(Green, j, i) << f(Blue, j, i);
        }

    }
}
