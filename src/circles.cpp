#include "program.h"
#include <cmath>

class Circles: public Program
{
struct Point
{
double x, y;

double dist(Point p2) {double xx = x - p2.x, yy = y - p2.y; return std::sqrt(xx*xx + yy*yy);}
};

Point origin = {2, 2};
double speed = 1e-6;
Picture pic = {};
long unsigned t = {};
int sh = 0; // to test inputs

public:
std::optional<Picture> update(unsigned delta_us, const Input& inputs) override
{
    //t += delta_us;

    if (inputs.size()) t += inputs[0];

    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            bool r = static_cast<int>(origin.dist({x - .3, y + .3}) + t) % 3 == 0;
            bool g = static_cast<int>(origin.dist({x + .1, y + .1}) + t) % 3 == 1;
            bool b = static_cast<int>(origin.dist({x - .3, y - .3}) + t) % 3 == 2;

            pic[x + COLS*y] = static_cast<Pixel>(RED*r + GREEN*g + BLUE*b);
        }
    }
    
    return pic;
}
};
