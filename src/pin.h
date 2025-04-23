#ifndef _PIN_H_
#define _PIN_H_

class Pin
{
// A class for aquiring
// ownership of some pin
public:
    Pin(unsigned n);

    Pin(const Pin&) = delete;
    Pin& operator=(const Pin&) = delete;

    Pin(Pin&& p): num{p.num} { p.num = 0; }
    Pin& operator=(Pin&& p) { unsigned temp = p.num; p.num = num; num = temp; return *this; }

    unsigned n() const { return num; }

    ~Pin();
private:
    // Fist bit (n = 0) is special
    // It is assigned to moved from pins
    inline static unsigned allocated {1};
    
    unsigned num;
};

class PinOut: public Pin
{
public:
    PinOut(unsigned n);

    // Write to pin
    bool operator=(bool);
};

#endif // _PIN_H_
