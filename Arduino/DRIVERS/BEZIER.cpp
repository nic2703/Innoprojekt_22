#include <Arduino.h>
#include "MT2_header.h"
#include "BEZIER_H.h"

// general formula is: B(t) = (1-t)^3*p_0 + 3*(1-t)^2*t*p1 + 3*(1-t)*t*p_2+t^3*p_3, 0<t<1
//TODO: modify for custumisation of plotter object
BEZIER::BEZIER()// : Plotter(0.0f, 0.0f, 1, 1)//initialises with standard plotter object
{
}

BEZIER::~BEZIER(){};

//should plot an r
bool BEZIER::robotica_test(int scale)
{
    Plotter::draw_line(0,               100 * scale,    255);
    Plotter::draw_line(20 * scale,      0,              255);
    Plotter::draw_line(0,               -65 * scale,    255);
    Plotter::draw_line(10 * scale,      10 * scale,     255);
    Plotter::draw_line(10 * scale,      5 * scale,      255);
    Plotter::draw_line(25 * scale,      0,              255);
    Plotter::draw_line(10 * scale,      -5 * scale,     255);
    Plotter::draw_line(10 * scale,      -10 * scale,    255);
    Plotter::draw_line(15 * scale,      15 * scale,     255);
    Plotter::draw_line(-10 * scale,     10 * scale,     255);
    Plotter::draw_line(-20 * scale,     7 * scale,      255);
    Plotter::draw_line(-25 * scale,     0,              255);
    Plotter::draw_line(-20 * scale,     -7 * scale,     255);
    Plotter::draw_line(-3 * scale,      -3 * scale,     255);
    Plotter::draw_line(0,               10 * scale,     255);
    Plotter::draw_line(-20 * scale,     0 * scale,      255);
    Plotter::draw_line(0 * scale,       -50 * scale,    255);
}

/*
draws a circle segment, assuming the plotter head/carriage is aready on the radius. at too steep angles, the circle must be approximated, so 
there will be zig-zag lines at the parts of the circle, where the tangent
is roughly parallel to the x or y axis
@param theta,phi,radius,dir (RADIANS!!!!!!!) theta is the angle to the //TODO:what axis? ()-axis at which the circle segment will 
begin drawing from, phi the arc-length and radius the radius of the circle segment
*/
bool BEZIER::circle_segment_onr(double theta, double phi, double radius){
    const COORDS center = {cos(theta) * radius, sin(theta) * radius};
    COORDS current = {0,0};
    COORDS previous = {0,0};

    if (phi == 0) {Serial.println("That circle segment is a bit too short! Try a arc length of *not* zero"); return false;}

    if(phi > 0)
    {
        for (int i = 0; i < (int) phi * 10.0; ++i)
        {
            current.x = center.x + radius * cos(theta + (double) i/10.0);
            current.y = center.y + radius * sin(theta + (double) i/10.0);
            draw_line(current.x - previous.x, current.y-previous.y, 255);
            previous.x = current.x;
            previous.y = current.y;
        }
    } else if (phi < 0)
    {
        for (int i = 0; i > (int) phi * 10.0; --i)
        {
            current.x = center.x + radius * cos(theta + (double) i/10.0);
            current.y = center.y + radius * sin(theta + (double) i/10.0);
            draw_line(current.x - previous.x, current.y-previous.y, 255);
            previous.x = current.x;
            previous.y = current.y;
        }
    }
    return true;
}

/*
draws circle when plotter head/carriage is not currently on the arc of the circle
center_x and center_y is relative to current plotter position.
*/
bool BEZIER::circle_segment_offr(double theta, double phi, double radius, double center_x, double center_y){
    COORDS centre = {center_x, center_y};
    centre.x += cos(theta) * radius;
    centre.y += sin(theta) * radius;
    draw_line(centre.x, centre.y, 255);
    circle_segment_onr(theta, phi, radius);
    return true;
}

/*
function to plot nic's random mandala. I do not yet uderstand the maths behind this or in fact know what the transform is,
but i made this function nonetheless.
@param your-mother your mother
*/
/* bool BEZIER::random_curve(){
    Basically this can be done in two ways. Either, you pass random parameters directly to the function, which makes it more interactive as
    the kids might be able to modify these in some way:

    There is the question of the random curves. I imagine two methods (first one easier) for this:
    1) Rotated coordinate system from polar to quadratic. Random continous functions on an interval with known start and end values may
    be inserted into "pie"-like slices of the Mandala and then repeated. This would require a coordinate transformation from cartesian
    to polar and back

    2) Using purely parametric curves, it is probably possible to create a compelling (or multiple) repeating patterns with some rotational similarity.
    We would then need to ensure that this polar representation form is turned into cartesian.

    Other notes to mind:
    Functions may not have too high of a curvature at any point/corners, and may not have too short segements
} */
