#include "PlotterV2.h"
#include "plt_maths.h"
#include "draw.h"

extern Servo servo;
extern Plt p_plot;

bool draw_line(const Vec delta)
{
    if (out_of_bounds((p_plot.pos + delta))) // check if the line would land out of bounds
        {
            complain_OOB();
            panic(__SHUTDOWN_OOB);
            return false;
        }
    else if (delta.x == 0 && delta.y == 0)
        return true; // no line necessary
    
    bool x_larger = delta.y < delta.x;

    int a = (x_larger)? delta.y : delta.x;
    int b = (x_larger)? delta.x : delta.y;

    float slope = float(a)/float(b); //Range [0,1]

    int bits_a = int(SPEED_TO_BITS(slope));

    if (delta.x != 0)
    {
        set_brakes(p_plot.pins_x[2], LOW);
    }
    if (delta.y != 0)
    {
        set_brakes(p_plot.pins_y[2], LOW);
    }

    set_speed((x_larger) ? p_plot.pins_x : p_plot.pins_y, 255);

    set_speed((x_larger) ? p_plot.pins_y : p_plot.pins_x, (a==b)?255:int(SPEED_TO_BITS(slope)));

    uint16_t t_to_dist = millis() + ((x_larger)? delta.x:delta.y)/_MAX_SPEED;

    while (millis() < t_to_dist) {/*Do Nothing*/}

    set_brakes(p_plot.pins_x[2], HIGH);
    set_brakes(p_plot.pins_y[2], HIGH);
    
    return true; // hasn't failed so far
}