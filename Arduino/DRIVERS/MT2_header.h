#ifndef MT2_H_
#define MT2_H_

#include <Arduino.h>
#include <Servo.h>

typedef unsigned int pin;
typedef unsigned int bit_speed;


// global variables and definitions
#define TIME_MAX    10000
#define _BRAKE_A    9
#define _BRAKE_B    8
#define _SPEED_A    3 
#define _SPEED_B     11
#define _DIR_A      12
#define _DIR_B      13

/* #ifndef PI //HACK: should already be defined but gonna do it anyway

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

//macros
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#endif // PI and other constants
*/

//macros
#define set_dir(a, p_dir) a > 0 ? digitalWrite(p_dir, HIGH) : digitalWrite(p_dir, LOW) 
#define convert_to_time(delta, radius) 1000.0f * (delta) / (2 * PI * radius)
#define update_pos(pos, delta) pos += delta

//TODO: Angles for servo state: pen_lifted, pen_down

const int RADIUS_RACK = 0; //TODO: UPDATE RADIUS
const int RADIUS_PULLEY = 0; //TODO: UPDATE RADIUS

//update
const float MINDIST = 1.0f; // minimum distance the motor can turn in mm, immutable for safety
const int X_BOUNDARY = 100;
const int Y_BOUNDARY = 100;

// is there a return home?

void set_speed(pin, bit_speed);
void set_brakes(pin, int);

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
    bool approximate_line(float xdelta, float ydelta);
    
    public:
        Plotter(float, float, uint32_t);
        ~Plotter();
    
    bool setpinX(pin, pin, pin);
    bool setpinY(pin, pin, pin);
    bool initServo(pin, pin, pin);     
    bool resetpos(float, float);
    bool draw_line(float, float, float);
};


#endif