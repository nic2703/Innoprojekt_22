#include "Simple_Plotter.h"

Plotter plotter;

int pins_x[3];
int pins_y[3];

void stop()
{
    /*Brake*/
    digitalWrite(BRAKE_A, HIGH);
    digitalWrite(BRAKE_B, HIGH);

    /*Set motor speeds to 0*/
    analogWrite(SPEED_A, 0);
    analogWrite(SPEED_B, 0);

    abort();
}

bool run_into_walls(int pins_x[3], int pins_y[3])
{
    /*Make sure B is off*/
    set_speed(pins_y, 0);


    set_speed(pins_x, 255); //Run A forward

    /*Run until button*/
    while (digitalRead(LIMIT) != HIGH) {} //Do Nothing

    /*Stop A*/
    set_speed(pins_x, 0);

    /*Start B*/
    set_speed(pins_y, 255);

    /*Run until button*/
    while (digitalRead(LIMIT) == LOW) {} //Do Nothing

    /*Stop B*/
    set_speed(pins_y, 0);

    /*Start A*/
    set_speed(pins_x, -255);

    int time = millis();

    while (digitalRead(LIMIT) == LOW) {} //Do Nothing
    int duration_x = millis()-time;

    /*Stop A*/
    set_speed(pins_x, 0);

    /*Start B*/
    set_speed(pins_y, -255);

    time = millis();
    while (digitalRead(LIMIT) == LOW) {} //Do Nothing
    int duration_y = millis() - time;

    /*Stop B*/
    set_speed(pins_y, 0);

    return duration_x > duration_y;
}

void calibrate()
{
    /*Set pins into corect modes*/
    pinMode(SPEED_A, OUTPUT);
    pinMode(SPEED_B, OUTPUT);
    pinMode(DIR_A, OUTPUT);
    pinMode(DIR_A, OUTPUT);
    pinMode(DIR_A, OUTPUT);
    pinMode(BRAKE_A, OUTPUT);
    pinMode(DIR_B, OUTPUT);

    /*Try this arrangement of pins to motors*/
    pins_x[0] = SPEED_A, pins_x[1] = DIR_A, pins_x[2] = BRAKE_A;
    pins_y[0] = SPEED_B, pins_y[1] = DIR_B, pins_y[2] = BRAKE_B;

    if (run_into_walls(pins_x, pins_y)){ //Switch if necessary
        pins_x[0] = SPEED_B, pins_x[1] = DIR_B, pins_x[2] = BRAKE_B;
        pins_y[0] = SPEED_A, pins_y[1] = DIR_A, pins_y[2] = BRAKE_A;
    }

    plotter.x = 0;
    plotter.y = 0;
    plotter.z = 0;

    /*Attach interrupt to the limit switches*/
    attachInterrupt(LIMIT, stop, RISING); //FIXME: will this work
}

void set_speed(int pins[3], int speed)
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

bool out_of_bounds(int dx, int dy)
{
    int new_x = plotter.x + dx;
    int new_y = plotter.y + dy;
    if (new_x < 0 || new_y < 0 || new_x > MAX_X || new_y > MAX_Y)
    {
        return true;
    }
    return false;
}

void draw_line(int dx, int dy)
{
    if (out_of_bounds(dx, dy))
    { // security
        stop();
    }

    if (dx == 0 && dy == 0)
    {
        return;
    }

    double norm = sqrt(sq(dx) + sq(dy));
    double n_dx = (dx / norm) * CORRECTION;
    double n_dy = dy / norm;

    n_dx = (n_dx >= n_dy) ? 1 : n_dx/n_dy; //scales the values, makes sure that the larger value is 1--> 255 bits, by multiplying both by the inverse of the larger one faster motor runs at 255, slower motor runs at a scaled value [0,1]
    n_dy = (n_dy >= n_dx) ? 1 : n_dy/n_dx;

    set_speed(pins_x, (n_dx > n_dy) ? 255 : int(speed_to_bits(n_dx)));
    set_speed(pins_y, (n_dy > n_dx) ? 255 : int(speed_to_bits(n_dy)));

    int eta = millis() + int(abs( ( (n_dx > n_dy) ? dx/MAX_SPEED_X : dy/MAX_SPEED_Y )));

    while (millis() < eta)

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    plotter.x += dx;
    plotter.y += dy;
}

//TODO: implement servo functionality