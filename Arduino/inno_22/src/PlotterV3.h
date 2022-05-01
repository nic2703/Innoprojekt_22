#ifndef PLT_H
#define PLT_H

#include "pmath.h"
#include "errors"
#include "plt_pins"

typedef byte pin;

#define MAX_X 100000000 //TODO: choose a value here, this is the short side
#define MAX_Y 100000000 //TODO: Choose a value here, this is the long side
#define MAX_SPEED_X 1 //TODO: Get a max speed calculation
#define MAX_SPEED_Y 1 //TODO: Get the max speed calc§ulation

#define CORRECTION 1 //TODO: get Correction

#define set_dir(p_dir, s) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards
#define sign(x) (((x)>0)-((x)<0)) //nice to have, return values {-1, 0, 1}
#undef abs
#define abs(x) ((x)*sign((x))) //better

void _init_servo();

class Plotter
{
public:
    Plotter();
   
    //bool is_active();
    void draw_line(long, long);
    void draw_line(const Vec &) [[deprecated("Use draw_line(int, int) instead.")]];

    void circle_segment(Vec &, int, double);

private:
    pin pins_x[3];
    pin pins_y[3];

    int x, y, angle;

    bool out_of_bounds(int, int);
    bool run_into_walls(pin[3], pin[3]);

};
typedef Plotter Plt;

#endif // !PLT_H