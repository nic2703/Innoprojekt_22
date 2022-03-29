#ifndef MT2_H_
#define MT2_H_
#include "Arduino.h"
#define TIME_MAX 10000

// global variables and definitions
typedef unsigned int pin;
const float MINDIST = 1.0f; // minimum distance the motor can turn in mm, immutable for safety
//add macros,constexpr and fn signatures 
// is there a return home?
//TODO: add map_speed for mapping byte input to acutal vals 30-255

class Plotter
{
public:
    Plotter(float xposition = 0.0f, float yposition = 0.0f);
    ~Plotter();
    bool setpinX(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
    bool setpinY(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
    bool setpinZ(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);     
    bool resetpos(float xposition, float yposition);
    bool moveline(float posx = 0.0f, float posy = 0.0f, float speed = 100);
    // set radii_cogwheels

private:
    float xpos = 0.0f, ypos = 0.0f;
    pin xpspd = 0, xpbrk = 0, xpdir = 0; // x: {pinspeed, brakestate, direction}
    pin ypspd = 0, ypbrk = 0, ypdir = 0; // y: {pinspeed, brakestate, direction}
    pin zpspd = 0, zpbrk = 0, zpdir = 0; // TODO: update for servo
    bool islifted = false;
    float radiusx = 0.0f, radiusy = 0.0f;

    // throws error if length of move is > 10 s
    bool straight_line_x(float xdelta);
    bool straight_line_y(float ydelta);
    bool special_line(float xdelta, float ydelta);
    bool normal_line(float xdelta, float ydelta);
};


#endif