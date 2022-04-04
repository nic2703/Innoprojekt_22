#include <Arduino.h>
#include <Servo.h>
#include "MT2_header.h"

void set_speed(pin motorPin, bit_speed speed){
    analogWrite(motorPin, speed);
}

//sets brakes
void set_brakes(pin pin, int state){ 
  digitalWrite(pin, state);
}

Servo pen_servo;

/*
Constructor
@param x,y,init_with_brakes positions default to 0; if thrid argument not 0, brakes are set to high
*/
Plotter::Plotter(float xposition = 0.0f, float yposition = 0.0f, int _init_w_brakes = 1)
{
    xpos = xposition;
    ypos = yposition;
    //Mordor A
    pinMode(_SPEED_A, OUTPUT); // speed pin
    pinMode(_DIR_A, OUTPUT); // direction pin  
    pinMode(_BRAKE_A, OUTPUT); // brake pin
    //Mordor B
    pinMode(_SPEED_A, OUTPUT); //speed pin
    pinMode(_DIR_B, OUTPUT); // direction pin
    pinMode(_BRAKE_B, OUTPUT); // brake pin

    if (_init_w_brakes){
        set_brakes(_BRAKE_B, HIGH); //engage both brakes
        set_brakes(_BRAKE_A, HIGH);
    }
}

Plotter::~Plotter(){}; // no destructor needed


/*
set pins
@param x output pins (headers) on Arduino, default to 3, 9, 12
@return boolean value true if operation succeeded, else false
*/
bool Plotter::setpinX(pin pin_speed = _SPEED_A, pin pin_brake = _BRAKE_A, pin pin_direction = _DIR_A)
{
    if (pin_speed > 30 || pin_brake > 30 || pin_direction > 30) // available pins
    {
        return false;
    }
    xpspd = pin_speed;
    xpbrk = pin_brake;
    xpdir = pin_direction;
    return true;
}

/*
set pins
@param y output pins (headers) on Arduino, default to 
@return boolean value true if operation succeeded, else false
*/
bool Plotter::setpinY(pin pin_speed = _SPEED_B, pin pin_brake = _BRAKE_B, pin pin_direction = _DIR_B)
{
    if (pin_speed > 30 || pin_brake > 30 || pin_direction > 30) // IDEA: if pins are taken (say by x or  servo), complain
    {
        return false;
    }
    ypspd = pin_speed;
    ypbrk = pin_brake;
    ypdir = pin_direction;
    return true;
}

bool Plotter::initServo(pin pin_servo = _SERVO_LATCH) 
{
    if (pin_servo > 30)
    {
        return false;
    }
    servo_p = pin_servo;
    pen_servo.attach(pin_servo);
    return true;
}

/*
resets private positions of motors
@param x,y current positions, sets to input parameters
@return boolean value true if operation succeeded, else false
*/
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

/*
draw lines, function decides which type of line to make
@param xposnew,yposnew,speed desired new positions of x and y coordinates, speed at which to execute move
@return boolean value true if operation succeeded, else false
*/
bool Plotter::draw_line(float xposnew = 0.0f, float yposnew = 0.0f, float speed = 100.0f)
{
    float xdelta = xposnew - xpos;
    float ydelta = yposnew - ypos;
    if (IS_TOO_SMALL(xdelta) && IS_TOO_SMALL(ydelta)){
        Serial.println("Line too short!");
    } 
    else if (IS_TOO_SMALL(xdelta)) 
    {
        straight_line_y(ydelta);
    }
    else if (IS_TOO_SMALL(ydelta))
    {
        straight_line_x(xdelta);
    }
    else if (diagonal_line(xdelta, ydelta))
    {
        UPDATE_POS(xpos, xdelta);
        UPDATE_POS(ypos, ydelta);
        return true;
    }
    else
    {
        Serial.println("Encoder error, x-y-positions may be incorrect");
        return false;
    }
}

// throws error if length of move is > 10 s
/*
@param xdelta distance in x to draw - forward if positive, backwards if negative
@return t/f boolen values; true if operation was successful, else false
*/
bool Plotter::straight_line_x(float xdelta) 
{
    SET_DIR(xdelta, xpdir); // set direction
    //whats this owo
    float ms = TO_TIME(xdelta, RADIUS_PULLEY); 
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
    SET_DIR(ydelta, ypdir); // set direction 

    float ms = TO_TIME(ydelta, RADIUS_RACK);
    float duedate = millis() + (uint64_t) ms;
    
    digitalWrite(ypbrk, LOW); // release the handbrake
    analogWrite(ypspd, 255);
    //TODO: replace with braking fn

    if (TIME_MAX < ms)
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

bool Plotter::diagonal_line(float xdelta, float ydelta)
{
    // higher delta always has the maximum speed of 255
    SET_DIR(xdelta, xpdir); 
    SET_DIR(ydelta, ypdir);

    if (3 * abs(ydelta / xdelta) < 1 || abs(ydelta / xdelta) > 3.0f)
    {
        float duedate = millis();
        if (abs(xdelta) > abs(ydelta)) // if x move is greater than y move
        {
            float ms = TO_TIME(xdelta, RADIUS_PULLEY);
            duedate += ms;
            digitalWrite(xpbrk, LOW); // release the handbrake
            analogWrite(xpspd, 255);    // full speed line, x
            digitalWrite(ypbrk, LOW); // release the handbrake
            analogWrite(ypspd, (ydelta / xdelta) * 255.0f); // make diagonal
        } else {
            float ms = (ydelta) / (2 * PI * RADIUS_RACK); // same here
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

    } else { // approximate line

        if (approximate_line(xdelta, ydelta)) 
        return true;
        else 
        return false;
    }
} 

/*
Some lines have a too steep angle to draw accurately; the speed at which one of the two motors 
needs to rotate is less than the minimum achievable speed. This is fixed by approximating the line with a zig.-zag pattern.
@param xdelta,ydelta the relative coordinates to plotter carriage to draw the line to
@return t/f boolena values; true if operation succeded, false if it failed
*/
bool Plotter::approximate_line(float xdelta, float ydelta) {

    if (abs(xdelta) > abs(ydelta))
        {
            float ms_x = 1000.0f * (xdelta * (MINDIST / (2.0f * ydelta))) / (2 * PI * RADIUS_PULLEY);  // distance in order to achieve a 1mm rise through the theoretical perfect rise
            if (ms_x > xdelta) // if desired distance is smaller than possible distance, draw a line parallel to x or y axis
            {
                straight_line_x(xdelta);
                straight_line_y(ydelta); // technically smaller than the MINDIST value
                return true;
            }
            else   // here comes the incredibly difficult zigzag pattern
            {
                int numiterations = (xdelta - ms_x) / (2 * ms_x);
                straight_line_x(ms_x);
                straight_line_y(MINDIST);
                for (int i = 0; i < numiterations; ++i)
                {
                    straight_line_x(2 * ms_x);
                    straight_line_y(MINDIST);
                }
                straight_line_x(xdelta - numiterations * ms_x);
                straight_line_y(ydelta - numiterations * MINDIST);
                return true;
            }
        }
        else
        {
            float ms_y = 1000.0f * (ydelta * (MINDIST / (2.0f * xdelta))) / (2 * PI * RADIUS_RACK); // distance in order to achieve a 1mm rise through the theoretical perfect rise
            if (ms_y > ydelta) //
            {
                straight_line_y(ydelta);
                straight_line_x(xdelta); // technically smaller than the MINDIST value
                return true;
            }
            else
            {
                // here comes the incredibly difficult zigzag pattern
                int numiterations = (ydelta - ms_y) / (2 * ms_y);
                straight_line_y(ms_y);
                straight_line_x(MINDIST);
                for (int i = 0; i < numiterations; i++)
                {
                    straight_line_y(2 * ms_y);
                    straight_line_x(MINDIST);
                }
                straight_line_y(ydelta - numiterations * ms_y);
                straight_line_x(xdelta - numiterations * MINDIST);
                return true;
            }
            return false;
        }
}
// implement lift pen
// implement set pen