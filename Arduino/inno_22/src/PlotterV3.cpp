#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#ifndef Servo_h
#include <Servo.h>
#endif // DEBUG

#include "PlotterV3.h"

#ifndef PLT_H
#error Plotter not defined
#endif // !PLT_h
#ifndef Servo_h
#error Servo not defined
#endif // !Servo_h

#ifndef PMTH
#include "pmath.h"
#endif // !PMTH
#ifndef ERRORS
#include "errors"
#endif // !ERRORS

Servo servo;

static inline void set_speed(const pin motor, int speed) { analogWrite(motor, speed); }
static inline void set_brakes(const pin motor, int state) { digitalWrite(motor, state); }

static void set_speed(const pin pins[3], int speed)
{
    if (speed == 0)
    {
        digitalWrite(pins[2], HIGH);
        analogWrite(pins[0], 0);

        return;
    }

    digitalWrite(pins[1], (speed < 0) ? LOW : HIGH);
    analogWrite(pins[0], abs(speed));

    digitalWrite(pins[2], LOW);
}

static void emergency_stop()
{
    /*Engage Brakes*/
    digitalWrite(_BRAKE_A, HIGH);
    digitalWrite(_BRAKE_B, HIGH);

    /*Cut Power to the motors*/
    analogWrite(_SPEED_A, 0);
    analogWrite(_SPEED_B, 0);

    abort();
}

Plotter::Plotter() 
{
    Serial.begin(9600);
    pinMode(_BRAKE_A, OUTPUT);
    pinMode(_BRAKE_B, OUTPUT);
    pinMode(_SPEED_A, OUTPUT);
    pinMode(_SPEED_B, OUTPUT);
    pinMode(_DIR_A, OUTPUT);
    pinMode(_DIR_B, OUTPUT);

    pins_x[0] = _SPEED_A, pins_x[1] = _DIR_A, pins_x[2] = _BRAKE_A;
    pins_y[0] = _SPEED_B, pins_y[1] = _DIR_B, pins_y[2] = _BRAKE_B;
    /* if (run_into_walls(pins_x, pins_y)){ //Switch if necessary
        pins_x[0] = _SPEED_B, pins_x[1] = _DIR_B, pins_x[2] = _BRAKE_B;
        pins_y[0] = _SPEED_A, pins_y[1] = _DIR_A, pins_y[2] = _BRAKE_A;
    } */

    x = 0;
    y = 0;
    attachInterrupt(_SWITCH, emergency_stop, RISING); 

}

bool Plotter::run_into_walls(pin pins_x[3], pin pins_y[3])
{
    /*Make sure B is off*/
    set_speed(pins_y, 0);


    set_speed(pins_x, 255); //Run A forward

    /*Run until button*/
    while (digitalRead(_SWITCH) != HIGH) {} //Do Nothing

    /*Stop A*/
    set_speed(pins_x, 0);

    /*Start B*/
    set_speed(pins_y, 255);

    /*Run until button*/
    while (digitalRead(_SWITCH) == LOW) {} //Do Nothing

    /*Stop B*/
    set_speed(pins_y, 0);

    /*Start A*/
    set_speed(pins_x, -255);

    int time = millis();

    while (digitalRead(_SWITCH) == LOW) {} //Do Nothing
    int duration_x = millis()-time;

    /*Stop A*/
    set_speed(pins_x, 0);

    /*Start B*/
    set_speed(pins_y, -255);

    time = millis();
    while (digitalRead(_SWITCH) == LOW) {} //Do Nothing
    int duration_y = millis() - time;

    /*Stop B*/
    set_speed(pins_y, 0);

    return duration_x > duration_y;
}

bool Plotter::out_of_bounds(int dx, int dy)
{
    int new_x = x + dx;
    int new_y = y + dy;
    if (new_x < 0 || new_y < 0 || new_x > MAX_X || new_y > MAX_Y)
    {
        return true;
    }
    return false;
}

void Plotter::draw_line(long dx, long dy)
{

    if (dx == 0 && dy == 0)
    {
        set_speed(pins_x, 0);
        set_speed(pins_y, 0);

        return;
    }

    double norm = sqrt(sq(dx) + sq(dy));
    double n_dx = (dx / norm) * CORRECTION;
    double n_dy = dy / norm;

    bool x_geq = abs(n_dx) >= abs(n_dy);
    bool y_geq = abs(n_dy) >= abs(n_dx);

    n_dx = (x_geq) ? sign(n_dx) : n_dx/abs(n_dy); //scales the values, makes sure that the larger value is 1--> 255 bits, by multiplying both by the inverse of the larger one faster motor runs at 255, slower motor runs at a scaled value [0,1]
    n_dy = (y_geq) ? sign(n_dy) : n_dy/abs(n_dx);

    set_speed(pins_x, ( (x_geq) ? 255 : int(speed_to_bits(n_dx)) ) * sign(n_dx) );
    set_speed(pins_y, ( (y_geq) ? 255 : int(speed_to_bits(n_dy)) ) * sign(n_dy) );

    int eta = millis() + int(abs( ( (n_dx > n_dy) ? dx/MAX_SPEED_X : dy/MAX_SPEED_Y )));

    while (millis() < eta) {}

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    x += dx;
    y += dy;
}

void Plotter::draw_line(const Vec & delta)
{
    if (delta == Vec(0, 0)) {
        set_speed(pins_x, 0);
        set_speed(pins_y, 0);

        return; // no line necessary
    }

    double norm = delta.norm();
    double n_dx = (delta._x() / norm) * CORRECTION;
    double n_dy = delta._y() / norm;
    
    bool x_geq = abs(n_dx) >= abs(n_dy);
    bool y_geq = abs(n_dy) >= abs(n_dx);

    n_dx = (x_geq) ? sign(n_dx) : n_dx/abs(n_dy); //scales the values, makes sure that the larger value is 1--> 255 bits, by multiplying both by the inverse of the larger one faster motor runs at 255, slower motor runs at a scaled value [0,1]
    n_dy = (y_geq) ? sign(n_dy) : n_dy/abs(n_dx);

    set_speed(pins_x, ( (x_geq) ? 255 : int(speed_to_bits(n_dx)) ) * sign(n_dx) );
    set_speed(pins_y, ( (y_geq) ? 255 : int(speed_to_bits(n_dy)) ) * sign(n_dy) );

    int eta = millis() + int( abs( ( (n_dx > n_dy) ? delta._x() / MAX_SPEED_X : delta._y() /MAX_SPEED_Y )));

    while (millis() < eta) {}

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    x += delta._x();
    y += delta._y();
}

void Plotter::bezier_q(long c1_x, long c1_y, long end_x, long end_y, uint8_t precision)
{
    if (0 >= precision || precision <= 20) {
        return;
    }
    int p_x, p_y;

    for (uint8_t i = 0; i < precision; ++i)
    {
        p_x = pmath::qbezier_x(x, c1_x, end_x, precision, i);
        p_y = pmath::qbezier_y(y, c1_y, end_y, precision, i);
        draw_line(p_x, p_y);
    }
}

void Plotter::circle_segment(Vec & midpoint, int radius, double arc)
{
/*  Vec to_rad = (midpoint - Vec(x,y)) - Vec(midpoint - Vec(x,y)) / Vec(midpoint - Vec(x,y)).norm();
    draw_line(to_rad); */

    if (-0.01 < arc && arc < 0.01)
    {
        Serial.println("That circle segment is a bit too short! Try a arc length of *not* zero");
        return;
    }

    if (arc > 0)
    {
        for (int i = 0; i < arc * 10; i++) // steps in hundredths of arc length.
        {  
        }
    }
    else if (arc < 0)
    {                                       // TODO: make this go the other way
        for (int i = 0; i < arc * 10; i++) // steps in hundredths of arc length.
        {
            
        }
    }
}