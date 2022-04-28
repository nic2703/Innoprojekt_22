#ifndef PLT_h
#define PLT_h

#include "pmath.h"
#include "errors"

typedef byte pin;
typedef enum
{
    _BRAKE_A = 9,
    _BRAKE_B = 8,
    _SPEED_A = 3,
    _SPEED_B = 11,
    _DIR_A = 12,
    _DIR_B = 13,
    _SERVO = 4
} pins;

#define set_dir(p_dir, s) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards
#define _MAX_SPEED INFINITY //FIXME no

void panic(volatile error_t);
void emergency_stop();
void finish();

const pin pins_x[3] = {_SPEED_A, _DIR_A, _BRAKE_A};
const pin pins_y[3] = {_SPEED_B, _DIR_B, _BRAKE_B};

void go();

void drawA(int);

bool draw_line(const Vec);

void _init_servo();

/* class Plotter
{
public:
   
    bool active();

private:
};
Plotter::Plotter(){};
typedef Plotter Plt; */

#endif // !PLT_h