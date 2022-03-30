#ifndef MT2_H_

#define MT2_H_

#include "Arduino.h"
#define TIME_MAX 10000

//macros
#define set_dir(a, p_dir) a > 0 ? digitalWrite(p_dir, HIGH) : digitalWrite(p_dir, LOW) 
#define convert_to_time(delta, radius) 1000.0f * (delta) / (2 * PI * radius)
#define update_pos(pos, delta) pos += delta

// global variables and definitions
typedef unsigned int pin;
typedef unsigned int bit_speed;
//update
const float MINDIST = 1.0f; // minimum distance the motor can turn in mm, immutable for safety
const int X_BOUNDARY = 100;
const int Y_BOUNDARY = 100;


// is there a return home?
//TODO: add map_speed for mapping byte input to acutal vals 30-255
// set radii_cogwheels

void set_speed(pin, int);
void set_brakes(pin, pin, int, bit_speed);

class Plotter{
    private:
    float xpos = 0.0f, ypos = 0.0f;
    pin xpspd = 0, xpbrk = 0, xpdir = 0; // x: {pinspeed, brakestate, direction}
    pin ypspd = 0, ypbrk = 0, ypdir = 0; // y: {pinspeed, brakestate, direction}
    pin zpspd = 0, zpbrk = 0, zpdir = 0; // TODO: update for servo
    bool islifted = false;
    float radiusx = 0.0f, radiusy = 0.0f;

    bool straight_line_x(float);
    bool straight_line_y(float);
    bool diagonal_line(float, float);
    
    public:
        Plotter(float, float);
        ~Plotter();
    
    bool setpinX(pin, pin, pin);
    bool setpinY(pin, pin, pin);
    bool setpinZ(pin, pin, pin);     
    bool resetpos(float, float);
    bool draw_line(float, float, float);


};


#endif