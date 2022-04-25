#include "PlotterV2.h"
#include "plt_maths.h"
#include "draw.h"

bool draw_line(const Vec delta)
{
    if (p_plot.pos.x + delta.x > p_plot.max.x
    || p_plot.pos.y + delta.y > p_plot.max.y
    || p_plot.pos.x + delta.x < 0 && p_plot.pos.y + delta.y) // check if the line would land out of bounds
        {
            //panic(security shutdown code)?
            return false;
        }
    else if (delta.x == 0 && delta.y == 0)
        return true; // no line necessary
    
    bool x_larger = delta.y < delta.x;

    int a = (x_larger)? delta.x:delta.y;
    int b = (x_larger)? delta.y:delta.x;

    int slope = a/b;

    int bits_a = int(SPEED_TO_BITS(slope));

    if (delta.x != 0)
    {
        set_brakes(p_plot.pins_x[2], LOW);
    }
    if (delta.y != 0)
    {
        set_brakes(p_plot.pins_y[2], LOW);
    }

    set_speed((x_larger)?p_plot.pins_x:p_plot.pins_y, 255);
    set_speed((x_larger)?p_plot.pins_y:p_plot.pins_x, int(SPEED_TO_BITS(slope)));

    while (time_not_reached) {/*Do Nothing*/}

    set_brakes(p_plot.pins_x[2], HIGH);
    set_brakes(p_plot.pins_y[2], HIGH);
    
    return true; // hasn't failed so far
}