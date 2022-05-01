#include <Arduino.h>
#include <Servo.h>
#include "MT2_header.h"

Servo pen_servo;

/*
sets speed of moter in byte value from 0 to 255
@param motorPin,speed latch at which motor is attached and byte value to write to pin
*/
inline void set_speed(pin motorPin, bit_speed speed)
{
    analogWrite(motorPin, speed);
}

/*
sets speed of moter in byte value from 0 to 255
@param brake_pin,state latch at whiich the brake for the motor is attached and wheter to switch brake on or off
*/
inline void set_brakes(pin pin, int state)
{
    digitalWrite(pin, state);
}

// speed conversions
//-----------------------------------------------------------------

/*
converts a bytevalue speed to a mm/s
@param in_speed double value of speed
@return out_speed
*/
inline double map_to_speed(double in_speed_bits)
{
    double out_speed = 1.012e-5 * cube(in_speed_bits) - 6.19e-3 * sq(in_speed_bits) + 1.332 * in_speed_bits - 37.24;
    return out_speed;
}

/*
converts a speed in mm/s to a bytevalue that can be written to the motor. this curve is an approximation but is fairly accurate
@param in_speed double value of speed in mm/s
@return out_speed a double value constrained to be between 40 and 255
*/
inline double map_to_bitspeed(double in_speed)
{
    double out_speed = 203.887 - 0.0658762 * pow(3718.33 * sqrt(2.1603e9 * sq(in_speed) - 2.71301e11 * in_speed + 8.52866e12) - 1.72824e8 * in_speed + 1.08521e10, 0.33333333) + 34970.7 / pow(3718.33 * sqrt(2.1603e9 * sq(in_speed) - 2.71301e11 * in_speed + 8.52866e12) - 1.72824e8 * in_speed + 1.08521e10, 0.33333333);
    return constrain(out_speed, 40.0, 255.0);
} // ive done some testing and this seems to work quite reliably as the inverse of map_to_speed

/*
converts speed to revolutions per microsecond
@param delta,dir converts distance to time, this overload assumes full speed (255)
@return ms returns time in mocroseconds
*/
double to_micros(double delta, char dir)
{
    double ms;

    switch (dir)
    {
    case 'x':
        ms = TO_REV(delta, RADIUS_PULLEY) / (map_to_speed(255) / RADIUS_PULLEY); // takes in speed in byte val and returns rps;
        break;
    case 'y':
        ms = TO_REV(delta, RADIUS_RACK) / (map_to_speed(255) / RADIUS_RACK);
        break;
    default:
        Serial.println("That is not a valid axis! Try x or y instead.");
    };

    return 1000000.0 * ms;
}

/*
converts speed to revolutions per microsecond
@param delta,dir converts distance to time, this overload lets input speed be y byte value
@return ms returns time in mocroseconds
*/
double to_micros(double delta, char dir, double speed)
{
    double ms;

    switch (dir)
    {
    case 'x':
        ms = TO_REV(delta, RADIUS_PULLEY) / (map_to_speed(BYTE_SPEED(speed)) / RADIUS_PULLEY); // takes in speed in byte val and returns rps;
        break;
    case 'y':
        ms = TO_REV(delta, RADIUS_RACK) / (map_to_speed(BYTE_SPEED(speed)) / RADIUS_RACK);
        break;
    default:
        Serial.println("That is not a valid axis! Try x or y instead.");
    };

    return 1000000.0 * ms;
}

// intitialisation
//-----------------------------------------------------------------

/*
Constructor
@param x,y,init_with_brakes positions default to 0; if thrid argument not 0, brakes are set to high
*/
Plotter::Plotter(float xposition /*= 0*/, float yposition /*= 0*/ /*, int _init_w_brakes /*= 1, int _init_w_servo /*= 1*/)
{
    xpos = xposition;
    ypos = yposition;
    // Mordor A
    pinMode(_SPEED_A, OUTPUT); // speed pin
    pinMode(_DIR_A, OUTPUT);   // direction pin
    pinMode(_BRAKE_A, OUTPUT); // brake pin
    // Mordor B
    pinMode(_SPEED_A, OUTPUT); // speed pin
    pinMode(_DIR_B, OUTPUT);   // direction pin
    pinMode(_BRAKE_B, OUTPUT); // brake pin
    // servo
    servo_p = _SERVO_LATCH;
    pen_servo.attach(_SERVO_LATCH);
    pen_servo.write(0);

    set_brakes(_BRAKE_B, HIGH); // engage both brakes
    set_brakes(_BRAKE_A, HIGH);
}

Plotter::~Plotter(){}; // no destructor needed

/*
set pins
@param x output pins (headers) on Arduino, default to 3, 9, 12
@return boolean value true if operation succeeded, else false
*/
bool Plotter::setpinX(pin pin_speed, pin pin_brake, pin pin_direction)
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
bool Plotter::setpinY(pin pin_speed, pin pin_brake, pin pin_direction)
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

bool Plotter::setpinServo(pin pin_servo)
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

// line drawing fns
//-----------------------------------------------------------------

/*
sets servo to specified angle
@param angle input angle, between 0 and 1023
*/
bool Plotter::set_servo(int angle)
{
    // if (0 <= angle && angle <= 1023)
    //{
    // angle = TO_ANGLE(angle);
    pen_servo.write(angle);
    // return true;
    // } else return false;
    return true;
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
    if (IS_TOO_SMALL(xdelta) || IS_TOO_SMALL(ydelta))
    {
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

/*
throws error if length of move is > 10 s
@param xdelta distance in x to draw - forward if positive, backwards if negative
@return t/f boolen values; true if operation was successful, else false
*/
bool Plotter::straight_line_x(float xdelta)
{
    SET_DIR(xdelta, xpdir); // set direction

    uint32_t ms = to_micros(xdelta, 'x');

    if (TIME_MAX < ms)
    {
        Serial.println("Overran 10 second limit for straight line X move!");
        digitalWrite(xpbrk, HIGH);
        return false; // returns before the while loop runs
    }

    set_brakes(xpbrk, LOW); // release the hamdbrake
    set_speed(xpspd, 255);

    delayMicroseconds(ms);

    digitalWrite(xpbrk, HIGH);
    Serial.println("Straight line X move complete!");
    return true;
}

// same as straight_line_x
/*
@param xdelta distance in x to draw - forward if positive, backwards if negative
@return t/f boolen values; true if operation was successful, else false
*/
bool Plotter::straight_line_y(float ydelta)
{
    SET_DIR(ydelta, ypdir); // set direction

    uint32_t ms = to_micros(ydelta, 'y');

    if (TIME_MAX < ms)
    {
        Serial.println("Overran 10 second limit for straight line Y move!");
        digitalWrite(ypbrk, HIGH);
        return false;
    }

    set_brakes(ypbrk, LOW); // release the brake
    set_speed(ypspd, 255);

    delayMicroseconds(ms);

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
        float ms; // no uneccesary memory waste

        if (abs(xdelta) > abs(ydelta)) // if x move is greater than y move
        {
            ms = to_micros(xdelta, 'x');
            set_brakes(xpbrk, LOW);                       // release the handbrake
            set_speed(xpspd, 255);                        // full speed line, x
            set_brakes(ypbrk, LOW);                       // release the handbrake
            set_speed(ypspd, (ydelta / xdelta) * 255.0f); // make diagonal
        }
        else
        {
            ms = to_micros(ydelta, 'y');                  // same here
            set_brakes(ypbrk, LOW);                       // release the handbrake
            set_speed(ypspd, 255);                        // full speed line, y
            set_brakes(xpbrk, LOW);                       // release the handbrake
            set_speed(xpspd, (xdelta / ydelta) * 255.0f); // make diagonal
        }

        if (TIME_MAX < ms)
        {
            Serial.println("Overran 10 second limit for normal line move!");
            digitalWrite(xpbrk, HIGH);
            digitalWrite(ypbrk, HIGH);
            return false;
        }

        delay(ms);

        digitalWrite(xpbrk, HIGH);
        digitalWrite(ypbrk, HIGH);
        Serial.println("Normal_line move complete!");
        return true;
    }
    else
    { // approximate line

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
bool Plotter::approximate_line(float xdelta, float ydelta)
{

    if (abs(xdelta) > abs(ydelta))
    {
        float ms_x = 1000.0f * (xdelta * (MINDIST / (2.0f * ydelta))) / (2 * PI * RADIUS_PULLEY); // distance in order to achieve a 1mm rise through the theoretical perfect rise
        if (ms_x > xdelta)                                                                        // if desired distance is smaller than possible distance, draw a line parallel to x or y axis
        {
            straight_line_x(xdelta);
            straight_line_y(ydelta); // technically smaller than the MINDIST value
            return true;
        }
        else // here comes the incredibly difficult zigzag pattern
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
        if (ms_y > ydelta)                                                                      //
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