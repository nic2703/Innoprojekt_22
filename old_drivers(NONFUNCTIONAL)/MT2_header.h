#ifndef MT2_H_
#define MT2_H_

#include <Arduino.h>
#include <Servo.h>

typedef unsigned int pin;
typedef unsigned int bit_speed;


// global variables and definitions
#define TIME_MAX    10000000
#define _BRAKE_A    9
#define _BRAKE_B    8
#define _SPEED_A    3 
#define _SPEED_B    11
#define _DIR_A      12
#define _DIR_B      13
#define _SERVO_LATCH 4

//macros
#define SET_DIR(a, p_dir) (a > 0 ? digitalWrite(p_dir, HIGH) : digitalWrite(p_dir, LOW)) //if delta is negative, go backwards, else go forwards
#define TO_REV(delta, radius) ((delta) / (2 * PI * radius)) // converts distance to revolutions of the cog wheel 
#define UPDATE_POS(pos, delta) (pos += delta) //doesn't need doc
#define IS_TOO_SMALL(a) (abs(a) <= MINDIST) // is the distance smaller than the minimum achievable distance?
#define TO_ANGLE(bit_angle) (map(bit_angle, 0, 1023, 0, 179)) // maps a bit_angle between 0 and 1023 to an anoutput angle 
// 0 t0 179 degrees
#define BYTE_SPEED(nin_speed) (map(nin_speed, 0.0, 255.0, 40.0, 255.0)) // maps a byte speed to acutal achievable speed, (integer value)
#define cube(x) ((x)*(x)*(x))

/* reminder that these are at your disposal

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.2831853071795864769252S86766559
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

*/

const double RADIUS_RACK = 4.5;  // in mm (y)
const double RADIUS_PULLEY = 0.5;  // in mm (x)

//update
const float MINDIST = 0.00001lazygit; // minimum distance the motor can turn in mm, immutable for safety
const int X_BOUNDARY = 100;
const int Y_BOUNDARY = 100;

// TODO:is there a return home?

void set_speed(pin, bit_speed);
void set_brakes(pin, int);
double map_to_speed(double);
double map_to_bitspeed(double);
double to_micros(double, char);
double to_micros(double, char, double);

class Plotter{
    private:
    //pins for components
    float xpos = 0.0f, ypos = 0.0f;
    pin xpspd = 0, xpbrk = 0, xpdir = 0; // x: {pinspeed, brakestate, direction}
    pin ypspd = 0, ypbrk = 0, ypdir = 0; // y: {pinspeed, brakestate, direction}
    //servo pin and state
    pin servo_p = 0; 
    bool islifted = false;

    bool straight_line_x(float);
    bool straight_line_y(float);
    bool diagonal_line(float, float);
    bool approximate_line(float xdelta, float ydelta);
    
    public:
        Plotter(float xposition = 0.0f, float yposition = 0.0f/*, int _init_w_brakes = 1, int _init_w_servo = 1*/);
        ~Plotter();
    
    bool setpinX(pin pin_speed = _SPEED_A, pin pin_brake = _BRAKE_A, pin pin_direction = _DIR_A);
    bool setpinY(pin pin_speed = _SPEED_B, pin pin_brake = _BRAKE_B, pin pin_direction = _DIR_B);
    bool setpinServo(pin);
    bool set_servo(int);  
    bool resetpos(float, float);
    bool draw_line(float, float, float);
};

#endif