#include <Arduino.h>
#include <Servo.h>

#include "PlotterV3.h"

#ifndef PLT_h
#error Plotter not defined
#endif // !PLT_h

#ifndef PMTH
#include "pmath.h"
#endif // !PMTH
#ifndef ERRORS
#include "errors"
#endif // !ERRORS

static inline void set_speed(pin motor, int speed) {analogWrite(motor, speed);}
static inline void set_speed(pin motors[2], int speed) {
    set_dir(motors[1], speed);
    analogWrite(motors[0], abs(speed));
}
static inline void set_brakes(pin motor, int state) {digitalWrite(motor, state);}

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
    delay(10);

    /*Stop execution*/
    abort();
}

void finish()
{
    Serial.println("Program terminated with exit code: 0");
    delay(10);
    abort();
}

bool draw_line(const Vec delta)
{
    if (delta == Vec(0, 0))
    return true; // no line necessary
    
    bool x_larger = delta._y() < delta._x();

    int a = (x_larger)? delta._y() : delta._x();
    int b = (x_larger)? delta._x() : delta._y();

    float slope = float(a)/float(b); //Range [0,1]

    int bits_a = int(SPEED_TO_BITS(slope));

    if (delta._x() != 0)
    {
        set_brakes(_BRAKE_A, LOW);
    }
    if (delta._y() != 0)
    {
        set_brakes(_BRAKE_B, LOW);
    }

    set_speed((x_larger) ? p_plot.pins_x : p_plot.pins_y, 255);

    set_speed((x_larger) ? p_plot.pins_y : p_plot.pins_x, (a==b)?255:int(SPEED_TO_BITS(slope)));

    uint16_t t_to_dist = millis() + ((x_larger)? delta._x():delta._y())/_MAX_SPEED;

    while (millis() < t_to_dist) {/*Do Nothing*/}

    set_brakes(p_plot.pins_x[2], HIGH);
    set_brakes(p_plot.pins_y[2], HIGH);
    
    return true; // hasn't failed so far
}