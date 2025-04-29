#include "picostd.h"
#include "display.h"

constexpr unsigned ROWS = 5;
constexpr unsigned COLS = 6;

constexpr int DATA_PIN = 20;
constexpr int LATCH_PIN = 19;
constexpr int CLOCK_PIN = 18;
constexpr int BLINKY_PIN = 25;
constexpr int BUTTON_PIN = 21;

namespace anim
{
    // Global timer
    unsigned t = 0;

    bool kind = 0;

    auto vert_rainbow = [](Color c, int x, int y) -> bool
    {
        switch ( (x + t) % 3 )
        {
        case 0: return true;
        case 1: return c == Blue;
        case 2: return c == Red; 
        }
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

extern "C" void button_callback(pstd::uint gpio, pstd::uint32_t events)
{
    anim::t = 0;
    anim::kind = !anim::kind;
}

void setup()
{
    pstd::stdio_init_all();

    display = new Display<ROWS, COLS> {Shift {DATA_PIN, LATCH_PIN, CLOCK_PIN}};

    pstd::add_alarm_in_ms( 5000, alarm_callback, NULL, false );

    // TODO remove manual pin init
    pstd::gpio_init( BUTTON_PIN );
    pstd::gpio_pull_up( BUTTON_PIN );
    pstd::gpio_set_irq_enabled_with_callback( BUTTON_PIN, pstd::GPIO_IRQ_EDGE_RISE, true, &button_callback) ;
}

void loop()
{
    display->draw( anim::kind ? anim::horz_rainbow : anim::vert_rainbow );
    //anim::t++;
    //pstd::sleep_us( 100 );
    for (int i = 0; i < 100000; i++) ;
}

int main()
{
    setup();
    for (;;) loop();
}
