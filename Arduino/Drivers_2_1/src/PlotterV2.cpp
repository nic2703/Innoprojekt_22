#include "PlotterV2.h"

#ifndef PLT
#error Plotter not defined
#endif // !PLTs

inline void set_speed(pin motor_a, int speed_a)
{
    analogWrite(motor_a, speed_a);
}

inline void set_brakes(pin motor, int state)
{
    digitalWrite(motor, state);
}

void finish()
{
    Serial.println("Program terminated with exit code: 0");
}

/* void emergency_stop()
{
    panic(3);
} */

void panic(uint8_t error)
{
    /*Engage Brakes*/
    set_brakes( _BRAKE_A, HIGH);
    set_brakes( _BRAKE_B, HIGH);

    /*Cut Power to the motors*/
    set_speed( _SPEED_A, 0);
    set_speed( _SPEED_B, 0);

    /*Output the error*/
    Serial.println("Aborted: Error code: ");
    Serial.println(error);

    /*Wait for the message to be sent*/
    delay(10);

    /*Stop execution*/
    abort();
}

void _init_servo() // [[maybe_unused]]
{
    servo.attach(_SERVO);
    Serial.println("attached servo to pin 4");
}

Plt::Plt()
{
    if (!(__plt_init())) panic(1);
}

Plt::~Plt() {}

bool Plt::__plt_init()
{
    // run into walls till button registered
    x_speed = _SPEED_A, x_dir = _DIR_A, x_brk = _BRAKE_A;
    y_speed = _SPEED_B, y_dir = _DIR_B, y_brk = _BRAKE_B;

    calibrate();

    attachInterrupt(digitalPinToInterrupt(2), emergency_stop, )
}

void Plt::calibrate()
{
    done_c = true;
}