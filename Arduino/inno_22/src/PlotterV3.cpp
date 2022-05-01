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

// Servo servo;

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

static void panic(volatile error_t error)
{
    /*Engage Brakes*/
    digitalWrite(_BRAKE_A, HIGH);
    digitalWrite(_BRAKE_B, HIGH);

    /*Cut Power to the motors*/
    analogWrite(_SPEED_A, 0);
    analogWrite(_SPEED_B, 0);

    /*Output the error*/
    Serial.print("Aborted: Error code: ");
    Serial.println(error);

    /*Wait for the message to be sent*/
    delay(50);

    /*Stop execution*/
    abort();
}

static void finish()
{
    Serial.println("Program terminated with exit code: 0");
    delay(10);
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
    angle = 0;
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

/* void Plotter::draw_line(long dx, long dy)
{
    if (out_of_bounds(dx, dy))
    { // security
        emergency_stop();
    }

    if (dx == 0 && dy == 0)
    {
        return;
    }

    double norm = sqrt(sq(dx) + sq(dy));
    double n_dx = (dx / norm) * CORRECTION;
    double n_dy = dy / norm;

    n_dx = (n_dx >= n_dy) ? 1 : n_dx / n_dy; // scales the values, makes sure that the larger value is 1--> 255 bits, by multiplying both by the inverse of the larger one faster motor runs at 255, slower motor runs at a scaled value [0,1]
    n_dy = (n_dy >= n_dx) ? 1 : n_dy / n_dx;

    set_speed(pins_x, (n_dx > n_dy) ? 255 : int(speed_to_bits(n_dx)));
    set_speed(pins_y, (n_dy > n_dx) ? 255 : int(speed_to_bits(n_dy)));

    int eta = millis() + int(abs(((n_dx > n_dy) ? dx / MAX_SPEED_X : dy / MAX_SPEED_Y)));

    while (millis() < eta) {}

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    x += dx;
    y += dy;
} */
void Plotter::draw_line(int dx, int dy)
{
    if (out_of_bounds(dx, dy))
    { // security
        emergency_stop();
    }

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

    n_dx = (x_geq) ? sign(n_dx) : n_dx/n_dy; //scales the values, makes sure that the larger value is 1--> 255 bits, by multiplying both by the inverse of the larger one faster motor runs at 255, slower motor runs at a scaled value [0,1]
    n_dy = (y_geq) ? sign(n_dy) : n_dy/n_dx;

    set_speed(pins_x, ( (x_geq) ? 255 : int(speed_to_bits(n_dx)) ) * sign(n_dx) );
    set_speed(pins_y, ( (y_geq) ? 255 : int(speed_to_bits(n_dy)) ) * sign(n_dy) );

    int eta = millis() + int(abs( ( (n_dx > n_dy) ? dx/MAX_SPEED_X : dy/MAX_SPEED_Y )));

    while (millis() < eta)

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    x += dx;
    y += dy;
}

void Plotter::draw_line(const Vec & delta)
{
    if (delta == Vec(0, 0))
    return; // no line necessary
    
    bool x_larger = abs(delta._y()) < abs(delta._x());

    int a = (x_larger)? delta._y() : delta._x();
    int b = (x_larger)? delta._x() : delta._y();
    Serial.println(a);
    Serial.println(b);

    float slope = float(a)/float(b); //Range [0,1]
    Serial.println(slope);
    
    int bits_a = int(speed_to_bits(slope)) * ((a>0) ? 1 : -1);
    Serial.println(bits_a);

    if (delta._x() != 0)
    {
        set_brakes(_BRAKE_A, LOW);
    }
    
    if (delta._y() != 0)
    {
        set_brakes(_BRAKE_B, LOW);
    }

    set_speed((x_larger) ? pins_x : pins_y, 255 * ((b>0) ? 1 : -1));

    set_speed((x_larger) ? pins_y : pins_x, bits_a);

    uint16_t t_to_dist = millis() + ((x_larger)? abs(delta._x()):abs(delta._y()));
    Serial.println(t_to_dist);

    while (millis() < t_to_dist) {/*Do Nothing*/}

    set_brakes(pins_x[2], HIGH);
    set_brakes(pins_y[2], HIGH);
    
    return; // hasn't failed so far
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
            Vec
            sin(i);
            cos(i);   
        }
    }
    else if (arc < 0)
    {                                       // TODO: make this go the other way
        for (int i = 0; i < arc * 10; i++) // steps in hundredths of arc length.
        {
            
        }
    }
}