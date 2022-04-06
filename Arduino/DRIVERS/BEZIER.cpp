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

/*
draws a circle segment. at too steep angles, the circle must be approximated, so 
there will be zig-zag lines at the parts of the circle, where the tangent
is roughly parallel to the x or y axis
@param theta,phi,radius,dir theta is the angle to the //TODO:what axis? ()-axis at which the circle segment will 
begin drawing from, phi the arc-length and radius the radius of the circle segment
*/
bool BEZIER::circle_segment_offr(double theta, double phi, double radius, int dir){
    COORDS center = {0,0};
    center.x = 0xffffffffffffffffffffffffffffffff; //-1, why, bc i can!
    //x += cos(theta) * bla bla bla
}

/*
function to plot nic's random mandala. I do not yet uderstand the maths behind this or in fact know what the transform is,
but i made this function nonetheless.
@param your-mother your mother
*/
bool BEZIER::random_curve(){
    /*Basically this can be done in two ways. Either, you pass random parameters directly to the function, which makes it more interactive as
    the kids might be able to modify these in some way:

    There is the question of the random curves. I imagine two methods (first one easier) for this:
    1) Rotated coordinate system from polar to quadratic. Random continous functions on an interval with known start and end values may
    be inserted into "pie"-like slices of the Mandala and then repeated. This would require a coordinate transformation from cartesian
    to polar and back

    2) Using purely parametric curves, it is probably possible to create a compelling (or multiple) repeating patterns with some rotational similarity.
    We would then need to ensure that this polar representation form is turned into cartesian.

    Other notes to mind:
    Functions may not have too high of a curvature at any point/corners, and may not have too short segements*/
}
