#include <Arduino.h>
#include "MT2_header.h"
#include "BEZIER_H.h"

// general formula is: B(t) = (1-t)^3*p_0 + 3*(1-t)^2*t*p1 + 3*(1-t)*t*p_2+t^3*p_3, 0<t<1

BEZIER::BEZIER(/* args */)
{
    _init_coords();
}

BEZIER::~BEZIER()
{
}

void BEZIER::_init_coords(){
    curve.x = 0; // cursed return home
    curve.y = 0;
}


//should plot an r
bool BEZIER::robotica_test()
{
    Plotter::draw_line(0, 100, 255);
    Plotter::draw_line(20, 0, 255);
    Plotter::draw_line(0, -65, 255);
    Plotter::draw_line(10, 10, 255);
    Plotter::draw_line(10, 5, 255);
    Plotter::draw_line(25, 0, 255);
    Plotter::draw_line(10, -5, 255);
    Plotter::draw_line(10, -10, 255);
    Plotter::draw_line(15, 15, 255);
    Plotter::draw_line(-10, 10, 255);
    Plotter::draw_line(-20, 7, 255);
    Plotter::draw_line(-25, 0, 255);
    Plotter::draw_line(-20, -7, 255);
    Plotter::draw_line(-3, -3, 255);
    Plotter::draw_line(0, 10, 255);
    Plotter::draw_line(-20, 0, 255);
    Plotter::draw_line(0, -50, 255);
}

