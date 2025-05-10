#include "common.h"
#include "display.h"
#include "config.h"
#include "program.h"
//#include "tetris.cpp"

constexpr int I2C_ADDR = 0x0E;

constexpr int CLOCK_PIN = 16;
constexpr int LATCH_PIN = 17;
constexpr int DATA_PIN = 18;
constexpr int BUTTON_PIN = 19;
constexpr int SDA_PIN = 20;
constexpr int SCL_PIN = 21;
constexpr int BLINKY_PIN = 25;

#define MILLION 1000000

Display* display;

unsigned delay = MILLION / DEFAULT_FPS;

Program* game;

void setup()
{
    stdio_init_all(); // pstd

    display = new Display {Shift {DATA_PIN, LATCH_PIN, CLOCK_PIN}};

    // TODO remove manual pin init

    // I2C
    gpio_init( SDA_PIN );
    gpio_init( SCL_PIN );
    gpio_set_function( SDA_PIN, GPIO_FUNC_I2C );
    gpio_set_function( SCL_PIN, GPIO_FUNC_I2C );
    i2c_init( i2c0, 100 * 1000 ); 
    i2c_set_slave_mode( i2c0, true, I2C_ADDR );
}

void loop()
{
    if (!game) { game = new Tetris; }

    if ( i2c_get_read_available( i2c0 ) > 0 ) {
        uint8_t buf {};
        i2c_read_raw_blocking( i2c0, &buf, 1 );
    }    

    Input in{};

    auto res = game.update(delay, in);

    if (res)
        display->draw( res.value() );
    else
        delete game;

    sleep_us( delay );
}

int main()
{
    setup();
    for (;;) loop();
}
