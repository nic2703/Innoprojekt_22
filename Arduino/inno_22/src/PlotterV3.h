#ifndef PLT_H
#define PLT_H

#include "pmath.h"
#include "errors"
#include "plt_pins"

typedef byte pin;

#define MAX_X 1000 //TODO: choose a value here, this is the short side
#define MAX_Y 10000 //TODO: Choose a value here, this is the long side
#define MAX_SPEED_X 0.18 //TODO: Get a max speed calculation
#define MAX_SPEED_Y 0.32 //TODO: Get the max speed calculation

#define CORRECTION 1 //TODO: get Correction

#define set_dir(p_dir, s) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards

void panic(volatile error_t);
void emergency_stop();
void finish();

void _init_servo();

class Plotter
{
public:
    Plotter();
   
    bool is_active();
    void draw_line(int, int);

private:

    bool out_of_bounds(int, int);

    pin pins_x[3];
    pin pins_y[3];

    int x, y;
};
typedef Plotter Plt;

#endif // !PLT_H