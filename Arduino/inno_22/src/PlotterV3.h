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

void _init_servo();

class Plotter
{
public:
    Plotter();
   
    bool is_active();
    void draw_line(long, long);
    void draw_line(const Vec &) /*[[deprecated("Use draw_line(long, long) instead.")]]*/;

    void bezier_q(long, long, long, long, uint8_t);
    void bezier_c(long, long, long, long, long, long, uint8_t);
    void circle_seg(Vec &, int, double, int);
    void spiral(Vec &); //<--
    void move(long, long);
    void move(Vec &);

    void up();
    void down();

private:
    pin pins_x[3];
    pin pins_y[3];

    long x, y;
    int angle;
    const bool on = true;

    bool out_of_bounds(int, int);
    bool run_into_walls(pin[3], pin[3]);

    void servo_angle(int);
};
typedef Plotter Plt;

#endif // !PLT_H