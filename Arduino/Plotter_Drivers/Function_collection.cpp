#include "Plotter.h"

Plotter plotter; // instance of struct Plotter
extern Servo pen_lift;

// simple stuff
/*
inline void set_speed(pin motor_a, int speed_a, pin motor_b, int speed_b)
{
    analogWrite(motor_a, speed_a);
    analogWrite(motor_b, speed_b);
}
*/ //when writing to both motors it is not necessary to actually specify the motors

inline void set_speed(int speed_a, int speed_b)
{
    analogWrite(SPEED_A, speed_a);
    analogWrite(SPEED_B, speed_b);
}

// overload

inline void set_speed(pin motor, int speed)
{
    analogWrite(motor, speed);
}

// mafs 

double norm(vec vec)
{
    return sqrt(vec*vec);
}

uint16_t to_micros(vec delta)
{
return delta.x * M_1_PI;
} // formula delta/(pi*d)

// drawing
//--------------------------------

bool draw_line(vec delta)
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

bool draw_circle_segment(vec midpoint, int radius, double arc)
{
    if (/*condition to check if it will be out of bounds*/ true) // TODO: condition check if circle will be out of bounds
        return false;

    double current_angle = 0;
    vec to_midpoint = midpoint - plotter.position;
    vec to_point;

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

bool draw_circle_segment(vec midpoint, int radius, double arc, double start_angle)
{
    if (/*condition to check if it will be out of bounds*/ true) // TODO: condition check if circle will be out of bounds
        return false;

    double current_angle = 0;
    vec to_midpoint = midpoint - plotter.position;
    vec radius_angles = {int(cos(start_angle) * radius), int(sin(start_angle) * radius)}; //FIXME: find a better name for this variable, it is just the vector to the point ont the circle from the circle midpoint
    //FIXME: typecasting to int might cause severe precision issues here, please verify
    vec to_point = to_midpoint + radius_angles; //{midpoint.x - plotter.position.x + cos(start_angle) * radius, 2 * midpoint.y - plotter.position.y + sin(start_angle) * radius}; // vector to point on arc

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
