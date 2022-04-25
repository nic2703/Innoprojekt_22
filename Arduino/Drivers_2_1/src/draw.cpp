#include "PlotterV2.h"
#include "plt_maths.h"
#include "draw.h"

bool draw_line(const Vec delta)
{
    if (p_plot.pos.x + delta.x > p_plot.max.x
    || p_plot.pos.y + delta.y > p_plot.max.y
    || p_plot.pos.x + delta.x < 0 && p_plot.pos.y + delta.y) // check if the line would land out of bounds
        return false;
    else if (delta.x == 0 && delta.y == 0)
        return true; // no line necessary

    return true; // hasn't failed so far
}