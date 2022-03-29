#include "Arduino.h"
#include "MT2_header.h"

/*
Constructor
@param x and y position, default to 0
*/
Plotter::Plotter(float xposition = 0.0f, float yposition = 0.0f)
{
    xpos = xposition;
    ypos = yposition;
}

Plotter::~Plotter(){}; // no destructor


/*
set pins
@param x and y output pins (headers) on Arduino, default to 0
@return boolean value true if operation succeeded, else false
*/
bool Plotter::setpinX(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0)
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

bool Plotter::setpinY(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0)
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

bool Plotter::setpinZ(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0) // FIXME: modify for servo
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
    if (xposition > X_BOUNDARY || yposition > Y_BOUNDARY || xposition < 0 || yposition < 0) // condition for coords to be in plane
    {
        xpos = xposition;
        ypos = yposition;
        return true;
    }
    return false;
}

// line fn for deciding which line to make
bool Plotter::moveline(float xposnew = 0.0f, float yposnew = 0.0f, float speed = 100.0f) // TODO: modify for normal_line handling both noral and special lines?
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

// throws error if length of move is > 10 s
bool Plotter::straight_line_x(float xdelta) 
{
    set_dir(xdelta, xpdir); // set direction

    float ms = 1000.0f * (xdelta) / (2 * PI * radiusx);
    uint64_t duedate = millis() + (uint64_t) ms; // do the decimal points actually make a diffference
                                                // millis is unsigned long mate

    digitalWrite(xpbrk, LOW); // release the hamdbrake
    // TODO: braking fn
    analogWrite(xpspd, 255);

    if (TIME_MAX < ms)
    {
        Serial.println("Overran 10 second limit for straight line X move!");
        digitalWrite(xpbrk, HIGH);
        return false;   // returns before the while loop runs
    }
    
    while (millis() < duedate)
    {
        //delay(millis()); //please dont do this
    }
    digitalWrite(xpbrk, HIGH);
    Serial.println("Straight line X move complete!");
    return true;
}

    //same as straight_line_x
bool Plotter::straight_line_y(float ydelta)
{
    set_dir(ydelta, ypdir); // set direction

    float ms = 1000.0f * (ydelta) / (2 * PI * radiusy);
    float duedate = millis() + (uint64_t) ms;
    
    digitalWrite(ypbrk, LOW); // release the handbrake
    analogWrite(ypspd, 255);
    //TODO: replace with braking fn

    if (TIME_MAX > ms)
    {
        Serial.println("Overran 10 second limit for straight line Y move!");
        digitalWrite(ypbrk, HIGH);
        return false;
    }

    while (millis() < duedate)
    {
        // welcome to the waiting line
    }
    digitalWrite(ypbrk, HIGH);
    Serial.println("Straight line Y move complete!");
    return true;
}

bool Plotter::normal_line(float xdelta, float ydelta)
{
    // higher delta always has the maximum speed of 255
    set_dir(xdelta, xpdir); 
    set_dir(ydelta, ypdir);

    float duedate = millis();
    if (abs(xdelta) > abs(ydelta)) // if x move is greater than y move
    {
        float ms = convert_to_time(xdelta, radiusx);
        duedate += ms;
        digitalWrite(xpbrk, LOW); // release the handbrake
        analogWrite(xpspd, 255);    // full speed line, x
        digitalWrite(ypbrk, LOW); // release the handbrake
        analogWrite(ypspd, (ydelta / xdelta) * 255.0f); // make diagonal
    } else {
        float ms = (ydelta) / (2 * PI * radiusy); // same here
        duedate += ms;
        digitalWrite(ypbrk, LOW); // release the handbrake
        analogWrite(ypspd, 255);    // full speed line, y 
        digitalWrite(xpbrk, LOW); // release the handbrake
        analogWrite(xpspd, (xdelta / ydelta) * 255.0f); // make diagonal
    }

    float currenttime = millis();
    if (duedate - currenttime > TIME_MAX){
        Serial.println("Overran 10 second limit for normal line move!");
        digitalWrite(xpbrk, HIGH);
        digitalWrite(ypbrk, HIGH);
        return false;
    }

    while (millis() < duedate)
    {
        // welcome to the waiting line
    }

    digitalWrite(xpbrk, HIGH);
    digitalWrite(ypbrk, HIGH);
    Serial.println("Normal_line move complete!");
    return true;
} 
// TODO: decide whether it wouldn't make just as much sense implementing this as a safety in normal_line
bool Plotter::special_line(float xdelta, float ydelta) //FIXME: Fix this
{
    set_dir(xdelta, xpdir); 
    set_dir(ydelta, ypdir);
    if (abs(xdelta) > abs(ydelta))
    {
        float msx = 1000.0f * (xdelta * (MINDIST / (2.0f * ydelta))) / (2 * PI * radiusx); // distance in order to achieve a 1mm rise through the theoretical perfect rise
        if (msx > xdelta)
        {
            straight_line_x(xdelta);
            straight_line_y(ydelta); // technically smaller than the MINDIST value
        }
        else
        {
            // here comes the incredibly difficult zigzag pattern
            int numiterations = (xdelta - msx) / (2 * msx);
            straight_line_x(msx);
            straight_line_y(MINDIST);
            for (int i = 0; i < numiterations; ++i)
            {
                straight_line_x(2 * msx);
                straight_line_y(MINDIST);
            }
            straight_line_x(xdelta - numiterations * msx);
            straight_line_y(ydelta - numiterations * MINDIST);
        }
    }
    else
    {
        float msy = 1000.0f * (ydelta * (MINDIST / (2.0f * xdelta))) / (2 * PI * radiusy); // distance in order to achieve a 1mm rise through the theoretical perfect rise
        if (msy > xdelta)
        {
            straight_line_y(ydelta);
            straight_line_x(xdelta); // technically smaller than the MINDIST value
        }
        else
        {
            // here comes the incredibly difficult zigzag pattern
            int numiterations = (ydelta - msy) / (2 * msy);
            straight_line_y(msy);
            straight_line_x(MINDIST);
            for (int i = 0; i < numiterations; i++)
            {
                straight_line_y(2 * msy);
                straight_line_x(MINDIST);
            }
            straight_line_y(ydelta - numiterations * msy);
            straight_line_x(xdelta - numiterations * MINDIST);
        }
    }
}
// implement lift pen
// implement set pen