#include "picostd.h"
#include "display.h"

constexpr unsigned ROWS = 3;
constexpr unsigned COLS = 3;

constexpr int DATA_PIN = 20;
constexpr int LATCH_PIN = 19;
constexpr int CLOCK_PIN = 18;
constexpr int BLINKY_PIN = 25;

namespace anim
{
    // Global timer
    unsigned t = 0;

    auto vert_rainbow = [](Color c, int x, int y) -> bool
    {
        return (x + c + t) % 3 == 0;
    };

    auto horz_rainbow = [](Color c, int x, int y) -> bool
    {
        return (y + c + t) % 3 == 0;
    };
}

extern "C" pstd::int64_t alarm_callback(pstd::alarm_id_t id, void *user_data) {
    anim::t++;
    
    //pstd::add_alarm_in_us(500, alarm_callback, NULL, false);
    return 500000;
}

Display<ROWS, COLS>* display;

void setup()
{
    pstd::stdio_init_all();

    display = new Display<ROWS, COLS> {Shift {DATA_PIN, LATCH_PIN, CLOCK_PIN}};

    pstd::add_alarm_in_ms(5000, alarm_callback, NULL, false);
}

void loop()
{
    display->draw(anim::vert_rainbow);
    //anim::t++;
    //pstd::sleep_us( 100 );
    for (int i = 0; i < 100000; i++) ;
}

int main()
{
    setup();
    for (;;) loop();
}
