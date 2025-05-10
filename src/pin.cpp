#include "picostd.h"

#include "pin.h"

Pin::Pin(unsigned nn)
{
    num = ( allocated & (1 << nn) ) ? 0 : nn;
    allocated |= 1 << num;
}

Pin::~Pin()
{
    if ( !num ) return;
    allocated &= !( 1 << num );
}

PinOut::PinOut(unsigned nn): Pin{nn}
{
    if ( !n() ) return;

    gpio_init( n() ); // pstd
    gpio_set_dir( n(), true ); //pstd
}

bool PinOut::operator=(bool a)
{
    if ( !n() ) return a;

    gpio_put( n(), a ); // pstd
    return a;
}
