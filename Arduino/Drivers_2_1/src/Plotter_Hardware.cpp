#include "PlotterV2.h"

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
    abort();
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
    abort();
}