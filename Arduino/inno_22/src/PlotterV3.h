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
    _DIR_B = 13
} pins;

pin pins_x[3] = {_SPEED_A, _DIR_A, _BRAKE_A};
pin pins_y[3] = {_SPEED_B, _DIR_B, _BRAKE_B};

#define set_dir(p_dir, s) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards
#define _MAX_SPEED INFINITY //FIXME no

void panic(volatile error_t);
void emergency_stop();
void finish();

bool draw_line(const Vec);


#endif // !PLT_h