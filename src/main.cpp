#include <vector>
#include <array>
#include "picostd.h"
#include "display.h"

constexpr unsigned ROWS = 3;
constexpr unsigned COLS = 3;

constexpr int DATA_PIN = 20; // пин данных 
constexpr int LATCH_PIN = 19; // пин защелки 
constexpr int CLOCK_PIN = 18; // пин тактов синхронизации 
constexpr int BLINKY_PIN = 25; // пин встроенного светодиода

std::vector<int> a {0b1000000001110000, 0b0001000001110000, 0b0000001001110000};
std::vector<int> b {0b1000000001110000,0b0001000000100000, 0b0000001001110000, 0b0001000001110000};
std::vector<int> c {0b0100000001110000,0b0000100000100000, 0b0000000101110000, 0b0000100001110000};
std::vector<int> d {0b0010000001110000,0b0000010000100000, 0b0000000011110000, 0b0000010001110000};
std::vector<int> x {0b1000000001110000};
std::vector<int> e {0b1000000001000000, 0b0100000000100000, 0b0000100001000000, 0b0010000000010000, 0b0000010000100000, 0b0000001001000000, 0b0001000000010000, 0b0000001000100000, 0b0000000100100000};
std::vector<int> e1 {0b1000000001000000};
std::vector<int> e2 {0b0100000000100000, 0b0000100001000000};
std::vector<int> e3 {0b0010000000010000, 0b0000010000100000, 0b0000000011000000};
std::vector<int> e4 {0b0001000000010000, 0b0000001000100000};
std::vector<int> e5 {0b0000000100010000};

std::array<bool, ROWS*COLS> NONE = {};
std::array<bool, ROWS*COLS> H =
{
1, 0, 1,
1, 1, 1,
1, 0, 1
},
C = {
1, 1, 1,
1, 0, 0,
1, 1, 1
},
A = {
0, 1, 0,
1, 0, 1,
1, 0, 1
};


Display<ROWS, COLS>* display;

void setup()
{
    pstd::stdio_init_all();

    display = new Display<ROWS, COLS> {Shift {DATA_PIN, LATCH_PIN, CLOCK_PIN}};
}

/*
void animate( std::vector<int> sequence )
{
    for (int x: sequence)
    {
                
        shift->latch( 16 ) << x;
        pstd::sleep_ms( 50 );
        shift->latch( 16 ) << 0b0000000000000000;
        pstd::sleep_ms( 50 );
    }
}
*/

void loop()
{
    display->draw(C.data(), H.data(), A.data());
    //pstd::sleep_ms( 50 );
    //display->draw(NONE.data(), NONE.data(), NONE.data());
    //pstd::sleep_ms( 1 );
}

/*
void blink( int x )
{
    for (;;)
    {
        for (int i = 0; i < x; ++i)
        {
            gpio_put( blinky, 0 );
            sleep_ms( 200 );
            gpio_put( blinky, 1 );
            sleep_ms( 200 );
        }

        gpio_put( blinky, 0 );
        sleep_ms( 3000 );
    }
}
*/

int main()
{
    setup();
    for (;;) loop();
}
