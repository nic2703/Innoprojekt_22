#include "Arduino.h"
#include "MT2_header.h"


class Plotter
{
private:

//TODO: add map_speed for mapping byte input to acutal vals 30-255

    float xpos = 0.0f, ypos = 0.0f;
    pin xpspd = 0, xpbrk = 0, xpdir = 0; // x: {pinspeed, brakestate, direction}
    pin ypspd = 0, ypbrk = 0, ypdir = 0; // y: {pinspeed, brakestate, direction}
    pin zpspd = 0, zpbrk = 0, zpdir = 0; // TODO: update for servo
    bool islifted = false;
    float radiusx = 0.0f, radiusy = 0.0f;

    // throws error if length of move is > 10 s
    bool straight_line_x(float xdelta) 
    {
        if (xdelta > 0)
            digitalWrite(xpdir, HIGH);
        else
            digitalWrite(xpdir, LOW); // set direction

        float ms = 1000.0f * (xdelta) / (2 * PI * radiusx);
        uint64_t duedate = millis() + (uint64_t)ms; // do the decimal points actually make a diffference
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
    bool straight_line_y(float ydelta)
    {
        if (ydelta > 0)
            digitalWrite(ypdir, HIGH);
        else
            digitalWrite(ypdir, LOW); // set direction

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

    bool normal_line(float xdelta, float ydelta)
    {
        // higher delta always has the maximum speed of 255
        xdelta > 0 ? digitalWrite(xpdir, HIGH) : digitalWrite(xpdir, LOW); //TODO: set_direction() function needed
        ydelta > 0 ? digitalWrite(ypdir, HIGH) : digitalWrite(ypdir, LOW);

        float duedate = millis();
        if (abs(xdelta) > abs(ydelta)) // if x move is greater than y move
        {
            float ms = 1000.0f * (xdelta) / (2 * PI * radiusx); // TODO: convert_speed as macro or constexpr
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
    bool special_line(float xdelta, float ydelta)
    {
        xdelta > 0 ? digitalWrite(xpdir, HIGH) : digitalWrite(xpdir, LOW); //FIXME: Fix this
        ydelta > 0 ? digitalWrite(ypdir, HIGH) : digitalWrite(ypdir, LOW);

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
    public:
    Plotter(float xposition = 0.0f, float yposition = 0.0f);
    ~Plotter();
    bool setpinX(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
    bool setpinY(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
    bool setpinZ(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
    // set radii_cogwheels
    bool resetpos(float xposition, float yposition);
    bool moveline(float posx = 0.0f, float posy = 0.0f, float speed = 100);
    // implement lift pen
    // implement set pen
};

