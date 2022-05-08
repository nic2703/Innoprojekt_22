/**
 * @file PlotterV3.h
 * @authors Robotica 
 *          (vdamani@ethz.ch)
 *          (vkottas@ethz.ch)
 *          (nscheuer@ethz.ch)
 * @brief
 * @version 4.1
 * @date 2022-05-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PLT_H
#define PLT_H

#include "pmath.h"
#include "errors"
#include "plt_pins"

typedef byte pin;

#define MAX_X 4200 // 1000 is equivalent to 50 mm, so a4 is 4200 wide
#define MAX_Y 5940 // same here

#define MAX_SPEED_X 0.6449 // correction factor for speed, so 1000 as input is equivalent to 50 mm
#define MAX_SPEED_Y 1.547 

#define CORRECTION (1 / 2.0) // modifies diagonal lines to ensure that they are at the correct angle

class Plotter
{
public:
    Plotter();
    Plotter(long, long);

    bool is_active();
    void calibrate();

    void draw_line(long, long);
    void draw_line(const Vec &) /*[[deprecated("Use draw_line(long, long) instead.")]]*/;
    void draw_line(const Vec_d &) /*[[deprecated("Use draw_line(long, long) instead.")]]*/; // XXX: will cause narrowing of vector components as it calls draw_line(long, long)

    void bezier_q(long, long, long, long, uint8_t precision = 40);
    void bezier_c(long, long, long, long, long, long, uint8_t precision = 40);
    void circle_seg(Vec_d &, int, double, int);
    void b_circle_seg(int);

private:
    pin pins_x[3];
    pin pins_y[3];

    long x, y;
    const bool on = true; // well, i mean, as soon as the object is initialised, the plotter is *technically* on

    bool run_into_walls(pin[3], pin[3]);
};
typedef Plotter Plt;

#endif // !PLT_H