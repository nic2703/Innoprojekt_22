#include "MT2_header.h"
#include "MT2_MOTOR_DRIVER.cpp"

Plotter::Plotter(float xposition, float yposition)
{
    xpos = xposition;
    ypos = yposition;
}

Plotter::~Plotter(){}; // no destructor

bool Plotter::setpinX(pin pinspeed, pin pinbreak, pin pindirection)
{
    if (pinspeed > 30 || pinbreak > 30 || pindirection > 30) // available pins
    {
        return false;
    }
    xpspd = pinspeed;
    xpbrk = pinbreak;
    xpdir = pindirection;
    return true;
}

bool Plotter::setpinY(pin pinspeed, pin pinbreak, pin pindirection)
{
    if (pinspeed > 30 || pinbreak > 30 || pindirection > 30) // TODO: if pins are taken (say by x or  servo), complain
    {
        return false;
    }
    ypspd = pinspeed;
    ypbrk = pinbreak;
    ypdir = pindirection;
    return true;
}

bool Plotter::setpinZ(pin pinspeed, pin pinbreak, pin pindirection) // FIXME: modify for servo
{
    if (pinspeed > 30 || pinbreak > 30 || pindirection > 30)
    {
        return false;
    }
    zpspd = pinspeed;
    zpbrk = pinbreak;
    zpdir = pindirection;
    return true;
}
// reset private positions of motors
bool Plotter::resetpos(float xposition, float yposition)
{
    xpos = xposition;
    ypos = yposition;
    // can add condition for coords to be in plane later
    return true;
}

// line fn for deciding which line to make
bool Plotter::moveline(float xposnew, float yposnew, float speed) // TODO: modify for normal_line handling both noral and special lines?
{
    float xdelta = xposnew - xpos;
    float ydelta = yposnew - ypos;
    if (abs(xdelta) <= MINDIST)
    {
        straight_line_y(ydelta);
    }
    else if (abs(ydelta) <= MINDIST)
    {
        straight_line_x(xdelta);
    }
    else if (abs(ydelta / xdelta) < 1.0f / 3.0f || abs(ydelta / xdelta) > 3.0f)
    {
        // use normal line
        if (normal_line(xdelta, ydelta))
        {
            xpos += xdelta;
            ypos += ydelta;
            return true;
        }
        else
        {
            Serial.println("Encoder error, x-y-positions may be incorrect");
            return false;
        }
    }
    else
    {
        // use special line
        if (special_line(xdelta, ydelta))
        {
            xpos += xdelta;
            ypos += ydelta;
            return true;
        }
        else
        {
            Serial.println("Encoder error, x-y-positions may be incorrect");
            return false;
        }
    }
}