#include "program.h"

class Program
{
public:
virtual std::optional<std::array<Pixel, ROWS*COLS>> update(unsigned delta_us, const Input& inputs) = 0;

virtual unsigned short preferredFPS()
{
    return DEFAULT_FPS;
}

};

class Snake: public Program
{
struct Point {int x, y};

std::list<Point> snek = {{1, 1}, {2, 1}, {3, 1}};
int dir = 
};
