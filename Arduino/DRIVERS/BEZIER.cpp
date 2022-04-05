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

bool BEZIER::robotica_test(){
    Plotter::draw_line();
}

