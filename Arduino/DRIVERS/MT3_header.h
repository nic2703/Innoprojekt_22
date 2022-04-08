#ifndef MT3_H_
#define MT3_H_
#include <Arduino.h>
#include <Servo.h>

typedef unsigned int pin;
typedef unsigned int bit_speed;

//globals
#define _BRAKE_A    9
#define _BRAKE_B    8
#define _SPEED_A    3
#define _SPEED_B    11
#define _DIR_A      12
#define _DIR_B      13
#define _SERVOLATCH 10

const double RADIUS_RACK = 4.5;
const double RADIUS_PULLY = 0.5;

class Plotter{
    private:
        float xpos = 0.0f, ypos = 0.0f;
        pin xpspd = 0, xpbrk = 0, xpdir = 0;
        pin ypspd = 0, ypbrk = 0, ypdir = 0;
        pin servo_p = 0;
        bool islifted = false;

        bool straight_line_x(float);
        bool straight_line_y(float);
        bool diagonal_line(float, float);

    public:
        Plotter(float x = 0.0f, float y = 0.0f);
        ~Plotter();

        bool setpinX(pin, pin, pin);
        bool setpinY(pin, pin, pin);
        bool setpinS(pin);
        bool resetpos(float, float);
        bool draw_line(float, float, float)
};


#endif