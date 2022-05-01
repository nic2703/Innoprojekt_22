#include "Plotter.h"

Plotter plotter; // instance of struct Plotter
extern Servo pen_lift;


bool __plt_init()
{
    // TODO: do button check

    pinMode(buttonPin, INPUT);
    
    pinMode(ledPin, OUTPUT); // this 
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin, OUTPUT);



}

// simple stuff

inline void set_speed(int speed_a, int speed_b, bool both)
{
    analogWrite(motor_a, speed_a); // FIXME: global motor vars needed
    analogWrite(motor_b, speed_b);
}

inline void set_speed(pin motor, int speed)
{
    analogWrite(motor, speed);
}

uint16_t to_micros(Vec delta)
{
return delta.x * M_1_PI;
} // formula delta/(pi*d)

// drawing
//--------------------------------

bool draw_line(Vec delta)
{
    if (plotter.position.x + delta.x > plotter.max_position.x || plotter.position.y + delta.y > plotter.max_position.y || plotter.position.x + delta.x < 0 && plotter.position.y + delta.y) // check if the line would land out of bounds
        return false;
    else if (delta.x == 0 && delta.y == 0)
        return true;

    plotter.position += delta;

    int slope;
    
    if (delta.y)
        slope = delta.x / delta.y; // since these are in 100ths of mm, integer division does not greatly impact accuracy!
    else //do smth else

    if (slope == 1) {} // why do we need this ? 

    return true;
}

bool draw_circle_segment(Vec midpoint, int radius, double arc)
{
    if (/*condition to check if it will be out of bounds*/ true) // TODO: condition check if circle will be out of bounds
        return false;

    double current_angle = 0;
    Vec to_midpoint = midpoint - plotter.position;
    Vec to_point; //FIXME: needed definition for to_point, presumanly using the dot product

    if (-0.01 < arc && arc < 0.01)
    {
        Serial.println("That circle segment is a bit too short! Try a arc length of *not* zero");
        return false;
    }
    else if (draw_line(to_point))
        ;
    else
        return false;

    if (arc > 0)
    {
        for (int i = 0; i < arc * 100; i++) // steps in hundredths of arc length.
        {
            /* TODO: make nr of steps dependant on arc, implement drawing lines for each small segment */
        }
    }
    else if (arc < 0)
    {                                       // TODO: make this go the other way
        for (int i = 0; i < arc * 100; i++) // steps in hundredths of arc length.
        {
            /* TODO: make nr of steps dependant on arc, implement drawing lines for each small segment */
        }
    }
    return true;
}

//overload

bool draw_circle_segment(Vec midpoint, int radius, double arc, double start_angle)
{
    if (/*condition to check if it will be out of bounds*/ true) // TODO: condition check if circle will be out of bounds
        return false;

    double current_angle = 0;
    Vec to_midpoint = midpoint - plotter.position;
    Vec radius_angles = Vec(int(cos(start_angle) * radius), int(sin(start_angle) * radius)); //FIXME: find a better name for this variable, it is just the vector to the point ont the circle from the circle midpoint
    //FIXME: typecasting to int might cause severe precision issues here, please verify
    Vec to_point = to_midpoint + radius_angles; //{midpoint.x - plotter.position.x + cos(start_angle) * radius, 2 * midpoint.y - plotter.position.y + sin(start_angle) * radius}; // vector to point on arc

    if (-0.01 < arc && arc < 0.01)
    {
        Serial.println("That circle segment is a bit too short! Try a arc length of *not* zero");
        return false;
    }
    else if (draw_line(to_point))
        ;
    else
        return false;

    if (arc > 0)
    {
        for (int i = 0; i < arc * 100; i++) // steps in hundredths of arc length.
        {
            /* TODO: make nr of steps dependant on arc, implement drawing lines for each small segment */
        }
    }
    else if (arc < 0)
    {                                       // TODO: make this go the other way
        for (int i = 0; i < arc * 100; i++) // steps in hundredths of arc length.
        {
            /* TODO: make nr of steps dependant on arc, implement drawing lines for each small segment */
        }
    }
    return true;
}


bool lift_pen(int pos)
{
    if (plotter.z == pos)
        return false;

    // implement servo lifting/setting function here
    pen_lift.write(/*some angle*/ 10000000); //WARNING THIS IS A PLACEHOLDER VALUE

    plotter.z = pos;
    return true;
}
