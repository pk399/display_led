#include "program.h"

class Spiner: public Program
{
const int lag = 4;
const Pixel color = BLUE;
const int max_pos = ROWS*2 + COLS*2 - 4;

int pos = 0;

std::pair<int, int> to_xy(int pos)
{
    if (pos < COLS - 1) return {pos, 0};
    pos -= COLS - 1;

    if (pos < ROWS - 1) return {COLS - 1, pos};
    pos -= ROWS - 1;

    if (pos < COLS - 1) return {COLS - 1 - pos, ROWS - 1};
    pos -= COLS - 1;

    return {0, ROWS - 1 - pos};
}
 
public:
std::optional<Picture> update(unsigned delta_us, const Input& inputs) override
{
    Picture pic{};

    pos = (pos + 1) % max_pos;

    for (int i = 0; i < lag + 1; i++)
    {
        auto c = to_xy((max_pos + pos - i) % max_pos);

        pic[c.first + COLS*c.second] = color;
    }
    
    return pic;
}
};
