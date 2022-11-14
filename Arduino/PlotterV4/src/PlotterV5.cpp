/**
 * @file PlotterV3.cpp
 * @authors Robotica 
 *          (vdamani@ethz.ch)
 *          (vkottas@ethz.ch)
 *          (nscheuer@ethz.ch)
 * @brief
 * @version 4.1
 * @date 2022-05-04
 *
 * @copyright Copyright (c) 2022
 *
 */

// check that arduino IDE is installed
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#include "PlotterV5.h"

#ifndef PLT_H
#error Plotter not defined
#endif // !PLT_h

#ifndef PMTH
#include "pmath"
#endif // !PMTH
#ifndef ERRORS
#include "errors"
#endif // !ERRORS

// maths bc i dont want a seperate maths.cpp file, fight me // -- hello this is future me. it would seem i failed in that regard as i did end up making a pmath file lol
namespace pmath
{
    long cbez_x(long x, long c1_x, long c2_x, long end_x, uint8_t prec, uint8_t i)
    {
        double t = double(i) / prec;
        return (cube(1 - t) * x) + (sq(1 - t) * 3 * t * c1_x) + ((1 - t) * 3 * sq(t) * c2_x) + (cube(t) * end_x);
    }

    long cbez_y(long y, long c1_y, long c2_y, long end_y, uint8_t prec, uint8_t i)
    {
        double t = double(i) / prec;
        return (cube(1 - t) * y) + (sq(1 - t) * 3 * t * c1_y) + ((1 - t) * 3 * sq(t) * c2_y) + (cube(t) * end_y);
    }

    long qbez_x(long x, long c1, long end_x, uint8_t prec, uint8_t i)
    {
        double t = double(i) / prec;
        return (sq(1 - t) * x) + ((1 - t) * 2 * t * c1) + (sq(t) * end_x);
    }

    long qbez_y(long x, long c1, long end_x, uint8_t prec, uint8_t i)
    {
        double t = double(i) / prec;
        return (sq(1 - t) * x) + ((1 - t) * 2 * t * c1) + (sq(t) * end_x);
    }

    template <typename T>
    Vector<T> Vector<T>::orth() const
    {
        return Vector<T>(-y, x); // using the simple version -> vector is turned by 90° anticlockwise
    }

    template <typename T>
    double Vector<T>::norm() const
    {
        return sqrt(sq(x) + sq(y));
    }

    template <typename T>
    Vector<T> Vector<T>::rotate(double theta)
    {
        T temp_x = x;
        this -> x = round(x * cos(theta) - y * sin(theta));
        this -> y = round(temp_x * sin(theta) + y * cos(theta));
        return Vector<T>(x, y);
    }

    template <typename T>
    Vector<T> Vector<T>::post_rotate(double theta)
    {
        Vector<T> temp = Vector<T>(x, y);

        this -> x = round(x * cos(theta) - y * sin(theta));
        this -> y = round(temp.x * sin(theta) + y * cos(theta));

        return temp;
    }

} // namespace pmath

// hardware (speed and brakes)
static inline void set_speed(const pin motor, int speed) { analogWrite(motor, speed); }
static inline void set_brakes(const pin motor, int state) { digitalWrite(motor, state); }


static void set_speed(const pin pins[3], int speed)
{
    if (speed == 0)
    {
        digitalWrite(pins[2], HIGH);
        analogWrite(pins[0], 0);

        return;
    }

    digitalWrite(pins[1], (speed < 0) ? LOW : HIGH);
    analogWrite(pins[0], abs(speed));

    digitalWrite(pins[2], LOW);
}

// emergency stop
static void panic()
{
    /*Engage Brakes*/
    digitalWrite(_BRAKE_A, HIGH);
    digitalWrite(_BRAKE_B, HIGH);

    /*Cut Power to the motors*/
    analogWrite(_SPEED_A, 0);
    analogWrite(_SPEED_B, 0);

    exit(1);
}

// default constructor, will initialise to 0 0
Plotter::Plotter() : x(0), y(0)
{
    pinMode(_BRAKE_A, OUTPUT);
    pinMode(_BRAKE_B, OUTPUT);
    pinMode(_SPEED_A, OUTPUT);
    pinMode(_SPEED_B, OUTPUT);
    pinMode(_DIR_A, OUTPUT);
    pinMode(_DIR_B, OUTPUT);

    pinMode(_SWITCH, INPUT);
    
    pins_x[0] = _SPEED_A, pins_x[1] = _DIR_A, pins_x[2] = _BRAKE_A;
    pins_y[0] = _SPEED_B, pins_y[1] = _DIR_B, pins_y[2] = _BRAKE_B;

    // x = 0; //  186 * 7
    // y = 0; //  126 * 7
}

// constructor for custom coordinates, if required
Plotter::Plotter(long in_x, long in_y) : x(in_x), y(in_y)
{
    pinMode(_BRAKE_A, OUTPUT);
    pinMode(_BRAKE_B, OUTPUT);
    pinMode(_SPEED_A, OUTPUT);
    pinMode(_SPEED_B, OUTPUT);
    pinMode(_DIR_A, OUTPUT);
    pinMode(_DIR_B, OUTPUT);

    pinMode(_SWITCH, INPUT);

    pins_x[0] = _SPEED_A, pins_x[1] = _DIR_A, pins_x[2] = _BRAKE_A;
    pins_y[0] = _SPEED_B, pins_y[1] = _DIR_B, pins_y[2] = _BRAKE_B;
}


const int Plotter::pos_x() const
{
    return this->x;
}

const int Plotter::pos_y() const
{
    return this->y;
}



// calibration
void Plotter::calibrate()
{

    set_home(pins_x, pins_y); // actually go to (0, 0)

    delay(500);
    EIFR = 0x01;
    delay(500);

    this -> x = 0;
    this -> y = 0;
    
    attachInterrupt(digitalPinToInterrupt(_SWITCH), panic, FALLING); // making sure that if the carriage runs into walls , it will abort
}

// calibration check sequence
void Plotter::set_home(pin pins_x[3], pin pins_y[3])
{
    volatile const int BACK = 100; // backwards speed for bouncing off limit switches
    /*Make sure B is off*/
    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    while (digitalRead(_SWITCH) == LOW) {}
    
    set_speed(pins_x, 255); // Run A forward

    /*Run until button*/
    while (digitalRead(_SWITCH) == HIGH) {} // Do Nothing
    delay(10);// try debounce

    /*Run A BACK to ensure switch is not pressed*/
    set_speed(pins_x, -(BACK));

    while (digitalRead(_SWITCH) == LOW) {}

    /*Stop A*/
    set_speed(pins_x, 0);

    /*Start B*/
    set_speed(pins_y, 255);
    /*Run until button*/
    while (digitalRead(_SWITCH) == HIGH) {} // Do Nothing

    /*Run A BACK to ensure switch is not pressed*/
    set_speed(pins_y, -(BACK));

    while (digitalRead(_SWITCH) == LOW) {}

    /*Stop B*/
    set_speed(pins_y, 0);

    /*Start A*/
    set_speed(pins_x, -255);

    //uint8_t time = micros();
    while (digitalRead(_SWITCH) == HIGH) {} // Do Nothing
    //uint8_t duration_x = micros() - time;

    /*Run A BACK to ensure switch is not pressed*/
    set_speed(pins_x, BACK);

    while (digitalRead(_SWITCH) == LOW) {}

    /*Stop A*/
    set_speed(pins_x, 0);

    /*Start B*/
    set_speed(pins_y, -255);

    //time = micros();
    while (digitalRead(_SWITCH) == HIGH) {} // Do Nothing
    //uint8_t duration_y = micros() - time;

    /*Run A BACK to ensure switch is not pressed*/
    set_speed(pins_y, BACK);

    while (digitalRead(_SWITCH) == LOW) {}


    /*Stop B*/
    set_speed(pins_y, 0);
    set_speed(pins_x, 0);
    delay(100);

    //delay(500);

    return;
}

void Plotter::home()
{
    draw_line((100 - x), -y); // idk why assymetry, but this works
}

bool Plotter::is_active() const // imma be honest, this is kinda a joke lol
{
    return on; // well obvs
}

// uses some smart maths to draw lines
void Plotter::draw_line(long dx, long dy)
{
    if (dx == 0 && dy == 0)
    {
        set_speed(pins_x, 0);
        set_speed(pins_y, 0);
        return;
    }

    double d_dx = double(dx) * CORRECTION;
    double d_dy = double(dy);

    double norm = sqrt(sq(d_dx) + sq(d_dy));
    double n_dx = d_dx / norm;                //-->  1/sqrt(2)
    double n_dy = d_dy / norm; //--> 1/sqrt(2)*correction

    bool x_geq = abs(n_dx) >= abs(n_dy); //--> false
    bool y_geq = abs(n_dy) >= abs(n_dx); //--> true

    set_speed(pins_x, ((x_geq) ? 255 : speed_to_bits(n_dx / n_dy)) * sign(dx)); //--> sets speed to 0
    set_speed(pins_y, ((y_geq) ? 255 : speed_to_bits(n_dy / n_dx)) * sign(dy)); //--> sets speed to -255

    unsigned long eta = uint32_t(abs(((x_geq) ? d_dx / MAX_SPEED : d_dy / MAX_SPEED))); // apparently this works

    delay(eta);

    set_speed(pins_x, 0);
    set_speed(pins_y, 0);

    x += dx;
    y += dy;
}

// Overload for Vector<int>
void Plotter::draw_line(const Vec &delta)
{
    draw_line(delta._x(), delta._y());
}

// Overload for Vector<double>, WARNING: causes narrowing due to cast from type double to long, not that that makes a real difference here, but its generally unsafe
void Plotter::draw_line(const Vec_d &delta)
{
    draw_line(long(delta._x()), long(delta._y()));
}

// quadratic beziers
void Plotter::bezier_q(long c1_x, long c1_y, long end_x, long end_y, uint8_t precision)
{
    if (precision <= 0 || 50 < precision)
    {
        return;
    }
    const long start_x = x, start_y = y;
    long p_x, p_y;

    for (uint8_t i = 0; i <= precision; ++i) // why we go to precision + 1 idk
    {
        p_x = pmath::qbez_x(start_x, c1_x, end_x, precision, i) - x;
        p_y = pmath::qbez_y(start_y, c1_y, end_y, precision, i) - y;

        draw_line(p_x, p_y);
    }
    draw_line(end_x - x, end_y - y);
    return;
}

//cubic beziers
void Plotter::bezier_c(long c1_x, long c1_y, long c2_x, long c2_y, long end_x, long end_y, uint8_t precision)
{
    if (precision <= 0 || 50 < precision)
    {
        return;
    }
    const long start_x = x, start_y = y; 
    long p_x, p_y;

    for (uint8_t i = 0; i <= precision; ++i)
    {
        p_x = pmath::cbez_x(start_x, c1_x, c2_x, end_x, precision, i) - x;
        p_y = pmath::cbez_y(start_y, c1_y, c2_y, end_y, precision, i) - y;

        draw_line(p_x, p_y);
    }
    draw_line(end_x - x, end_y - y);
    return;
}

// FIXME: should draw a circle segment, will draw a lemon
void Plotter::circle_seg(Vec_d &m, int radius, double max_angle = 2 * PI, int precision = 20)
{
    {
        double norm = m.norm();                                                                      // calculate norm first for efficionk
        draw_line(int((m._x() - x) * (1 - radius / norm)), int((m._y() - y) * (1 - radius / norm))); // draw to the radius
    }

    double da = (2.0 * PI) / precision * sign(max_angle); // make sure the way is signed
    double agl = 0;                                       // init

    Vec_d v = Vec_d(x, y) - m;
    Vec_d temp;
    while (abs(agl) <= abs(max_angle))
    {
        temp = v;
        draw_line(temp - v.rotate(da)); // rotate vector and subtract to get a dv
        agl += da;                      // keep track of angle
    }
}
// difference in y and x accuracy makes this hard anyway, but idk what exactly is wrong with this implementation

// uses beziers to draw an approxumation of a cricle-arc from (0,1) to (1,0) (clockwise, unlike in maths)
void Plotter::b_circle_seg(int radius) 
{
    x = 0;
    y = radius; // the constant C is stolen from a random article on the internet, forgot thr link tho
    bezier_c(C * radius, radius, radius, C * radius, radius, 0, 20);
}

/* void Plotter::spiral(Vec & mid)
{

    Vec start = mid - Vec(x,y);
    move(start);
    //spiral stuff with orth vecs

} */