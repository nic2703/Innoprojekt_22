#include "PlotterV2.h"

#ifndef PLT
#error Plotter not defined
#endif // !PLTs

//----------------

inline bool out_of_bounds(Vec coords)
{
    return (coords.x <= 0 || coords.x >= _PAPER_WIDTH || coords.y <= 0 || coords.y >= _PAPER_LENGTH);
}

void complain_OOB(Vec coords) //[[maybe_unused]]
{
    Serial.print("ERROR 3: OOB (");
    Serial.print(coords.x + p_plot.pos.x);
    Serial.print(" , ");
    Serial.print(coords.y + p_plot.pos.y);
    Serial.print(")");
}

//---------------

Plt::Plt()
{
    if (!(__plt_init()))
        panic(1);
}

Plt::~Plt() {}

bool Plt::__plt_init()
{
    // run into walls till button registered
    pins_x[0] = _SPEED_A, pins_x[1] = _DIR_A, pins_x[2] = _BRAKE_A;
    pins_y[0] = _SPEED_B, pins_y[1] = _DIR_B, pins_y[2] = _BRAKE_B;

    if (calibrate()) // swap them
    {
        pins_y[0] = _SPEED_A, pins_x[1] = _DIR_A, pins_x[2] = _BRAKE_A;
        pins_x[0] = _SPEED_B, pins_y[1] = _DIR_B, pins_y[2] = _BRAKE_B;
    }

    attachInterrupt(digitalPinToInterrupt(BUTTON), emergency_stop, RISING);
}

bool Plt::calibrate()
{
    /*Calibrate Motor A*/
    set_brakes(pins_x[2], LOW);
    set_speed(pins_x, 255);

    /*run into wall*/
    while (!(digitalRead(BUTTON)))
    { /*DO NOTHING*/
    }
    set_brakes(pins_x[2], HIGH);

    /*run into the opposite wall*/
    set_speed(pins_x, -255);
    set_brakes(pins_x[2], LOW);

    int init_time = millis();
    while (!(digitalRead(BUTTON)))
    { /*DO NOTHING*/
    }
    duration_x = millis() - init_time; // measure time taken

    set_speed(pins_x, 0);
    set_brakes(pins_x[2], HIGH);

    /*Calibrate Motor B*/
    set_brakes(pins_y[2], LOW);
    set_speed(pins_y, 255);

    /*run into wall*/
    while (!(digitalRead(BUTTON)))
    { /*DO NOTHING*/
    }
    set_brakes(pins_y[2], HIGH);

    /*run into the opposite wall*/
    set_speed(pins_y, -255);
    set_brakes(pins_y[2], LOW);

    int init_time = millis();
    while (!(digitalRead(BUTTON)))
    { /*DO NOTHING*/
    }
    duration_y = millis() - init_time; // measure time taken

    set_speed(pins_y, 0);
    set_brakes(pins_y[2], HIGH);

    pos = Vec(0, 0);

    return (duration_x > duration_y); // returns wether or not x is the longer side
}