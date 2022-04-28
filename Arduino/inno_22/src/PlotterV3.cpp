#include <Arduino.h>
//#include <Servo.h>

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

//Servo servo;

static inline void set_speed(pin motor, int speed) {analogWrite(motor, speed);}
static inline void set_speed(const pin motors[], int speed) {
    set_dir(motors[1], speed);
    analogWrite(motors[0], abs(speed));
}
static inline void set_brakes(pin motor, int state) {digitalWrite(motor, state);}

void go() {
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

/* void _init_servo() // [[maybe_unused]]
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
 */

/*void drawA(int dist) {
     set_dir(_DIR_A, dist);
    set_speed(_SPEED_A);

} */

bool draw_line(const Vec delta)
{
    if (delta == Vec(0, 0))
    return true; // no line necessary
    
    bool x_larger = abs(delta._y()) < abs(delta._x());

    int a = (x_larger)? delta._y() : delta._x();
    int b = (x_larger)? delta._x() : delta._y();
    Serial.println(a);
    Serial.println(b);

    float slope = float(a)/float(b); //Range [0,1]
    Serial.println(slope);
    
    int bits_a = int(SPEED_TO_BITS(slope)) * ((a>0) ? 1 : -1);
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
    
    return true; // hasn't failed so far
}