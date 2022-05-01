#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
//  #include <Servo.h>
#endif

#include "PlotterV3.h"

#ifndef PLT_H
#error Plotter not defined
#endif // !PLT_h

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

Plotter::Plotter()
{
    Serial.begin(9600);
    pinMode(_BRAKE_A, OUTPUT);
    pinMode(_BRAKE_B, OUTPUT);
    pinMode(_SPEED_A, OUTPUT);
    pinMode(_SPEED_B, OUTPUT);
    pinMode(_DIR_A, OUTPUT);
    pinMode(_DIR_B, OUTPUT);
}

void emergency_stop()
{
    /*Engage Brakes*/
    digitalWrite(_BRAKE_A, HIGH);
    digitalWrite(_BRAKE_B, HIGH);

    /*Cut Power to the motors*/
    analogWrite(_SPEED_A, 0);
    analogWrite(_SPEED_B, 0);

    abort();
}

void panic(volatile error_t error)
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

void finish()
{
    Serial.println("Program terminated with exit code: 0");
    delay(10);
    abort();
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

void Plotter::draw_line(int dx, int dy)
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

    while (millis() < eta)

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    x += dx;
    y += dy;
}