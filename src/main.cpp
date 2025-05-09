#include "picostd.h"
#include "display.h"
#include "config.h"
#include "timer.h"

constexpr int I2C_ADDR = 0x0E;

constexpr int CLOCK_PIN = 16;
constexpr int LATCH_PIN = 17;
constexpr int DATA_PIN = 18;
constexpr int BUTTON_PIN = 19;
constexpr int SDA_PIN = 20;
constexpr int SCL_PIN = 21;
constexpr int BLINKY_PIN = 25;

namespace anim
{
    // Global timer
    unsigned t = 0;

    bool kind = 0;

    auto vert_rainbow = [](Color c, int x, int y) -> bool
    {
        switch ( (x + t) % 3 )
        {
        case 0: return true && !( t + x % COLS );
        case 1: return c == Blue;
        case 2: return c == Red; 
        }
    };

    auto horz_rainbow = [](Color c, int x, int y) -> bool
    {
        return (c == Blue) && (y + t) % 2 == 0;
    };
}

Display<ROWS, COLS>* display;
Timer* timer;

extern "C" void button_callback(pstd::uint gpio, pstd::uint32_t events)
{
    anim::t = 0;
    anim::kind = !anim::kind;
}

void setup()
{
    pstd::stdio_init_all();

    display = new Display<ROWS, COLS> {Shift {DATA_PIN, LATCH_PIN, CLOCK_PIN}};

    timer = new Timer(500000, []() -> bool {anim::t++; return true;});

    // TODO remove manual pin init

    // Button interrupt
    pstd::gpio_init( BUTTON_PIN );
    pstd::gpio_pull_up( BUTTON_PIN );
    pstd::gpio_set_irq_enabled_with_callback( BUTTON_PIN, pstd::GPIO_IRQ_EDGE_RISE, true, &button_callback) ;

    // I2C 
    pstd::gpio_init( SDA_PIN );
    pstd::gpio_init( SCL_PIN );
    pstd::gpio_set_function( SDA_PIN, pstd::GPIO_FUNC_I2C );
    pstd::gpio_set_function( SCL_PIN, pstd::GPIO_FUNC_I2C );
    pstd::i2c_init( &pstd::i2c0_inst, 100 * 1000 ); 
    pstd::i2c_set_slave_mode( &pstd::i2c0_inst, true, I2C_ADDR );
}

void loop()
{
    if ( pstd::i2c_get_read_available( &pstd::i2c0_inst ) > 0 ) {
        anim::kind = !anim::kind;
        pstd::uint8_t buf {};
        pstd::i2c_read_raw_blocking( &pstd::i2c0_inst, &buf, 1 );
    }    

    display->draw( anim::kind ? anim::horz_rainbow : anim::vert_rainbow );
    //anim::t++;
    //pstd::sleep_us( 100 );
    //for (int i = 0; i < 100000; i++) ;
}

int main()
{
    setup();
    for (;;) loop();
}
