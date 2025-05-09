#include "picostd.h"
#include "display.h"
#include "config.h"
#include "program.h"
#include "picture.h"

constexpr int I2C_ADDR = 0x0E;

constexpr int CLOCK_PIN = 16;
constexpr int LATCH_PIN = 17;
constexpr int DATA_PIN = 18;
constexpr int BUTTON_PIN = 19;
constexpr int SDA_PIN = 20;
constexpr int SCL_PIN = 21;
constexpr int BLINKY_PIN = 25;

#define MILLION 1000000

Display<ROWS, COLS>* display;

unsigned delay = MILLION / DEFAULT_FPS;

Program* game;

void setup()
{
    pstd::stdio_init_all();

    display = new Display<ROWS, COLS> {Shift {DATA_PIN, LATCH_PIN, CLOCK_PIN}};

    timer = new Timer(500000, []() -> bool {anim::t++; return true;});

    // TODO remove manual pin init

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
    if (!game) { game = new //TODO; }

    if ( pstd::i2c_get_read_available( &pstd::i2c0_inst ) > 0 ) {
        anim::kind = !anim::kind;
        pstd::uint8_t buf {};
        pstd::i2c_read_raw_blocking( &pstd::i2c0_inst, &buf, 1 );
    }    

    Input in{};

    auto res = game.update(delay, in);

    if (res)
        display->draw( pic::DrawArray(res.value()) );
    else
        delete game;

    pstd::sleep_us( delay );
}

int main()
{
    setup();
    for (;;) loop();
}
