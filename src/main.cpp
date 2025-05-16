#include "common.h"
#include "display.h"
#include "config.h"
#include "program.h"

#include "tetris.cpp"
#include "circles.cpp"

constexpr int UART_BAUD = 115200;

constexpr int CLOCK_PIN = 16;
constexpr int LATCH_PIN = 17;
constexpr int DATA_PIN = 18;
constexpr int BUTTON_PIN = 19;
constexpr int UART_TX_PIN = 20; // UART1
constexpr int UART_RX_PIN = 21;
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

    // UART
    gpio_init( UART_TX_PIN );
    gpio_init( UART_RX_PIN );
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(uart1, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(uart1, UART_RX_PIN));
    uart_init(uart1, UART_BAUD);
}

void loop()
{
    if (!game) { game = new Circles; }
    
    Input in{};

    while ( uart_is_readable( uart1 ) > 0 ) {
        uint8_t buf {};
        uart_read_blocking( uart1, &buf, 1 );
        in.push_back( buf );
    }

    auto res = game->update( delay, in );

    if (res)
        display->draw( res.value() );
    else
    {
        delete game;
        game = new Tetris;
    }

    sleep_us( delay );
}

int main()
{
    setup();
    for (;;) loop();
}
