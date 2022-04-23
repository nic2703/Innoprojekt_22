#ifndef PLT
#define PLT

#include "Arduino.h"
#include "Servo.h"
#include "plt_maths.h" 

typedef byte pin;
typedef pmath::Vector Vec;

/**
 * servo for pen lifing
 */
Servo servo;

void __init_servo();

// initial pins set
#define BRAKE_A 9
#define BRAKE_B 8

#define SPEED_A 3
#define SPEED_B 11

#define DIR_A 12
#define DIR_B 13

// macros
#define cube(x) ((x)*(x)*(x))                                                           // cubes x
#define SPEED_TO_BITS(s) ((28.97 + 3.402 * (s)-0.1736 * sq(s) + 0.003101 * cube(s)) * 67.7) // caluculates bit value needed for bits, takes float returns float in the range [0, 1]
#define BITS_TO_SPEED(s) (1.012e-5 * cube(s) - 6.19e-3 * sq(s) + 1.332 * s - 37.24)         // bytespeed to actual irl speed
#define SET_DIR(s, p_dir) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards


inline void set_speed(pin, int);
inline void set_brakes(pin, int);


class Plt
{
public:
    Plt();
    Plt(Plt &&) = default;
    Plt(const Plt &) = default;
    Plt &operator=(Plt &&) = default;
    Plt &operator=(const Plt &) = default;
    ~Plt();

    iterator(); // idk do we want this?

private:
    bool __plt_init(); // initialisation sequence

    // motor pins
    pin x_pin, x_speed, x_dir;
    pin y_pin, y_speed, y_dir;

};

#endif // !PLT