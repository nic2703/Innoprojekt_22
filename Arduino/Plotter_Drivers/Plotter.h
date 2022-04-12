/**
 * @file Plotter.h
 * @author Robotica (vdamani@ethz.ch)
 * @brief Codebase for 2022 Innoprijekt at ETHZ.
 * A standard library for running Robotica's plotter. Code for Arduino.
 * The plot is divided into 1/100 mm x 1/100 mm integer segments, accuracy is not a problem.
 * @version 0.1
 * @date 2022-04-10
 */

#pragma once

#ifndef PLT_H
#define PLT_H

#include <Arduino.h>
#include <Servo.h>

#include "Plotter_maths.h"

typedef byte pin; // redefine pin datatype for readability

Servo pen_lift;

#define __LONGUP // long side of the plotter is "up" -> short is x, long is y-axis
//#define __SHORTUP // short side of the plotter is "up" -> long is x, short is y-axis

#ifdef __LONGUP

#define BRAKE_A 9
#define BRAKE_B 8

#define SPEED_A 3
#define SPEED_B 11

#define DIR_A 12
#define DIR_B 13

#elif __SHORTUP

#define BRAKE_A 8
#define BRAKE_B 9

#define SPEED_A 11
#define SPEED_B 3

#define DIR_A 13
#define DIR_B 12

#endif // ORIENTATION

//TODO: we need to replace all instances of vec struct with Vec class

class plt
{
public:
    plt() = default; // default constructor
    plt(plt &&) = default; 
    plt(const plt &) = default;
    plt &operator=(plt &&) = default;
    plt &operator=(const plt &) = default;
    ~plt() = default;

private:
};

//-------------------------------------------------------

#define MOTOR_CORRECTION a / b // TODO: we need to experimentally find a, b or a/b

// macros
#define cube(x) ((x)*(x)*(x))                                                           // cubes x
#define SPEED_TO_BITS(s) ((28.97 + 3.402 * (s)-0.1736 * sq(s) + 0.003101 * cube(s)) * 67.7) // caluculates bit value needed for bits, takes float returns float in the range [0, 1]
#define BITS_TO_SPEED(s) (1.012e-5 * cube(s) - 6.19e-3 * sq(s) + 1.332 * s - 37.24)         // bytespeed to actual irl speed
#define SET_DIR(s, p_dir) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                     // if delta is negative, go backwards, else go forwards
#define SET_BRAKES(brake, state) digitalWrite(brake, state)                                 // sets or releases the brakes of the specified motor

// data----------------------------------------------------

/**
 * @brief Defines a datatype for safekeeping of all relevant variables
 * stores position, maxima and servo data
 */
struct Plotter
{
    vec max_position = {21000, 29700}; // Short side of the paper with 1/100 mm precision, Long side of the paper with 1/100 mm precision
    vec position = {-1, -1};           // Current position of the plotter head
    int z = LOW;                          // Tells you if the pen is lifted
    bool has_not_died = true;             // Security
};

bool __plt_init(); // initialisation sequence plotter

// motors----------------------------------------------------

/**
 * @brief set the speed of the desired motor
 * @param motor the motor to set
 * @param speed the bitvalue of the speed to set it to
 * @returns void
 **/
inline void set_speed(pin, int, pin, int);
// FIXME: I find the returns void commedy, but we can get rid of it

//uint16_t revolutions(vec); // TODO: convert distance to necessary revs at 255 byte speed
//different calculation method is probs better

// Drawing--------------------------------------------------

/**
 * @brief Draws a line from the current position using
 * @param dx delta x
 * @param dy delta y
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_line(vec);

/**
 * @brief Draws a line to the nearest point on the circle radius, than starts drawing a circle segent up to the specified angle
 * @param midpoint circle midpoint coordinates
 * @param radius radius
 * @param arc arc length in radians within range [-2*pi, 2*pi]
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_circle_segment(vec, int, double);

/**
 * @brief Draws a line to a specified point on the circle radius, than starts drawing a circle segent up to the specified angle
 * @param midpoint circle midpoint coordinates
 * @param radius radius
 * @param arc arc length in radians within range [-2*pi, 2*pi]
 * @param start_angle angle relative to x-axis at which to start the circle segment
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_circle_segment(vec, int, double, double);

/**
 * @brief
 * @param pos desired position of the pen
 * @returns true | false if the pen could be lifted or set
 **/
bool lift_pen(int);

#endif