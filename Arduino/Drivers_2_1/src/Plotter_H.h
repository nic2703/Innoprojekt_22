#if !defined(__cplusplus)
    #error c++ compiler needed
#endif // cpp compiler needed lol

#ifndef PLT
#define PLT

#include "Arduino.h"
#include "Servo.h"
#include "draw.h"
#include "plt_maths.h"

typedef byte pin;
typedef pmath::Vector Vec;

/**
 * servo for lifing pen
 */
Servo servo;

struct pen
{
    int angle; // angle of servo
    bool down; // to ensure that the servo will not spin the wrong way
};

void _init_servo() __ATTR_CONST__; // dunno why i did this but it's cool //XXX?
bool lift_pen(pen &);

// initial pins set
#define _BRAKE_A 9
#define _BRAKE_B 8

#define _SPEED_A 3
#define _SPEED_B 11

#define _DIR_A 12
#define _DIR_B 13

#define _SERVO 4

// bigguns:
// teildurchmesser current: 24mm
// previous: 19

// pulley: !übersetzung:
// current: 15.75 (on motor), 
// previous:

// macros
#define cube(x) ((x)*(x)*(x))                                                           // cubes x
#define SPEED_TO_BITS(s) ((28.97 + 3.402 * (s)-0.1736 * sq(s) + 0.003101 * cube(s)) * 67.7) // caluculates bit value needed for bits, takes float returns float in the range [0, 1]
#define BITS_TO_SPEED(s) (1.012e-5 * cube(s) - 6.19e-3 * sq(s) + 1.332 * s - 37.24)         // bytespeed to actual irl speed
#define SET_DIR(s, p_dir) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards

inline void set_speed(pin, int);
inline void set_brakes(pin, int);

void panic(uint8_t); //__ATTR_NORETURN__

// TODO: should this be in a namespace too with all the drawing fns? that way the user could define their own drawing fns...
// tbh i have no clue so yea someone better tell me

class Plt
{
public:
    Plt();
    Plt(Plt &&) = default;
    Plt(const Plt &) = default;
    Plt &operator=(Plt &&) = default;
    Plt &operator=(const Plt &) = default;
    ~Plt();

private:
    bool __plt_init(pin); // initialisation sequence

    // motor pins
    pin x_speed, x_brk, x_dir;
    pin y_speed, y_brk, y_dir;
};

#endif // !PLT