/**
 * @file Plotter_maths.h
 * @author Robotica (vkottas@student.ethz.ch)
 * @brief Provides linear algebra support in the context of the plotter.
 * @version 0.1
 * @date 2022-04-12
 * 
 */
#ifndef PLT_MATH_H
#define PLT_MATH_H

#include <Arduino.h>

struct vec // ig vec is fine
{
    int x;
    int y;

    /**
     * @brief defines addition on vectors
     *
     * @param param vector to add
     * @return vec sum of the two vectors
     */
    vec operator+(vec &param)
    {
        x += param.x;
        y += param.y;

        return *this;
    }

    vec &operator+=(vec &param)
    {
        this->x += param.x; // actual modification of variable
        this->y += param.y;

        return *this;
    }

    /**
     * @brief defines subtraction on vectors
     *
     * @param param vector to subtract
     * @return vec difference of the two vectors
     */
    vec operator-(vec &param)
    {
        x -= param.x;
        y -= param.y;

        return *this;
    }

    vec &operator-=(vec &param)
    {
        this->x -= param.x;
        this->y -= param.y;

        return *this;
    }

    /**
     * @brief calculates the dot product
     *
     * @param param vector to calculate the dot product with
     * @return int dot product of the two vectors
     */
    int operator*(const vec &param) // const qualifier not rlly needed but safer this way
    {
        return x * param.x + y * param.y;
    }

    vec operator*(const int param) // reference not needed here
    {
        x *= param;
        y *= param;

        return *this;
    }

    vec &operator*=(const int param) // again, no reference needed in the argument, but const is good
    {
        this->x *= param;
        this->y *= param;

        return *this;
    }

    /**
     * @brief extention of the == operator to vectors
     *
     * @param param vector to compare
     * @return true if the vectors are equal
     * @return false if the vectors are not
     */
    bool operator==(vec &param)
    {
        return (x == param.x && y == param.y);
    }

    /**
     * @brief
     *
     * @return vec of a vector perpendicular to this one
     */
    vec normal() // classic normal vector calculation as in linear algebra
    {
        int t = x;
        x = y;
        y = -t;

        return *this;
    }
};

/**
 * @brief calculates the norm of a vector
 * @param vec vector of type vec
 * @return length of the vector
 */
double norm(vec);

#endif