#if !defined(__cplusplus)
#error c++ compiler needed
#endif // cpp compiler needed lol

#ifndef PLT
#define PLT

#include "Arduino.h"
#include "Servo.h"
#include "draw.h"
#include "plt_maths.h"

#ifndef PLT_MATHS
#warning MATHS IS NOT REAL
#endif // !PLT_MATHS

typedef byte pin;
typedef pmath::Vector Vec;

//ERRORS
#define __SHOTDOWN_INIT_FAILED 1
#define __SHUTDOWN_EMERGENCY 2
#define __SHUTDOWN_OOB 3
#define __SHUTDOWN_OOPS 4

// initial pins set RESERVED FOR MOTORS
#define _BRAKE_A 9
#define _BRAKE_B 8

#define _SPEED_A 3
#define _SPEED_B 11

#define _DIR_A 12
#define _DIR_B 13

#define _SERVO 4
#define _SERVO_TOP 180
#define _SERVO_BOTTOM 0

#define BUTTON 2

//-------------------------
#define _PAPER_LENGTH 297
#define _PAPER_WIDTH 210

#define _MAX_SPEED INFINITY //FIXME: needs calculation 
//max irl speed

// macros
#define cube(x) ((x)*(x)*(x))                                                           // cubes x
#define SPEED_TO_BITS(s) (((s) == 0)?0:((28.97 + 3.402 * (s)-0.1736 * sq(s) + 0.003101 * cube(s)) * 67.7)) // caluculates bit value needed for bits, takes float returns float in the range [0, 1] ||| (0,1] --> [30, 255] + 0 --> 0
#define BITS_TO_SPEED(s) (1.012e-5 * cube(s) - 6.19e-3 * sq(s) + 1.332 * s - 37.24)         // bytespeed to actual irl speed 
#define set_dir(p_dir, s) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards

#define servo_up() servo_goto(_SERVO_TOP)
#define servo_down() servo_goto(_SERVO_BOTTOM)

struct [[maybe_unused]] pen
{
    int angle; // angle of servo
    bool down; // to ensure that the servo will not spin the wrong way
};

void _init_servo() __ATTR_CONST__;
inline void servo_goto(byte angle);
inline byte servo_angle() __ATTR_CONST__;

inline void set_speed(pin *, int);
inline void set_brakes(pin, int);

void finish(void);
void panic(volatile uint8_t); //__ATTR_NORETURN__
void emergency_stop();

inline bool out_of_bounds(Vec);
[[maybe_unused]] void complain_OOB();

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
    bool calibrate(void);

    Vec pos;
    // motor pins
    pin pins_x[3];
    pin pins_y[3];
    
private:
    bool __plt_init(); // initialisation sequence


    static int duration_x;
    static int duration_y;

};

extern Servo servo;
extern Plt p_plot = Plt();

#endif // !PLT