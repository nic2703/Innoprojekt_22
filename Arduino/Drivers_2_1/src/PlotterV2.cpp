#include "PlotterV2.h"

#ifndef PLT
#error Plotter not defined
#endif // !PLTs

//----------------


inline void set_speed(pin motor[], int speed)
{
    set_dir(motor[1], speed);
    analogWrite(motor[0], abs(speed));
}

inline void set_brakes(pin motor, int state)
{
    digitalWrite(motor, state);
}

// servo
/**
 * @brief initialises servo object
 */
void _init_servo() // [[maybe_unused]]
{
    servo.attach(_SERVO);
    Serial.println("attached servo to pin 4");
}

inline void servo_goto(byte angle)
{
    servo.write(angle);
}

inline byte servo_angle()
{
    return servo.read();
}

// stop
void finish()
{
    Serial.println("Program terminated with exit code: 0");
    delay(10);
    //abort();
}

void emergency_stop()
{
    /*Engage Brakes*/
    digitalWrite(_BRAKE_A, HIGH);
    digitalWrite(_BRAKE_B, HIGH);

    /*Cut Power to the motors*/
    analogWrite(_SPEED_A, 0);
    analogWrite(_SPEED_B, 0);

    //abort();
}

void panic(volatile uint8_t error)
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
    //abort();
}

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

    init_time = millis();
    while (!(digitalRead(BUTTON)))
    { /*DO NOTHING*/
    }
    duration_y = millis() - init_time; // measure time taken

    set_speed(pins_y, 0);
    set_brakes(pins_y[2], HIGH);

    pos = Vec(0, 0);

    return (duration_x > duration_y); // returns wether or not x is the longer side
}