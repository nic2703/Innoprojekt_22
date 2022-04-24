/**
 * @file vec.h
 * @author Robotica (vkottas@student.ethz.ch)
 * @brief Provides linear algebra support in the context of the plotter.
 * @version 0.1
 * @date 2022-04-12
 * 
 */
#pragma once

#ifndef VEC_H
#define VEC_H

#include <Arduino.h>

class Vec
{
public:
    int x; //x coordinate
    int y; //y coordinate

    /**
     * @brief Construct a new Vec object
     * 
     * @param init_x x coordinate
     * @param init_y y coordinate
     */
    Vec(int init_x, int init_y)
    {
        x = init_x;
        y = init_y;
    }

    /**
     * @brief defines == on vectors
     * 
     * @param vector 
     * @return true if the two vectors are equal
     * @return false if they are not
     */
    bool operator==(Vec vector)
    {
        return vector.x == x && vector.y == y;
    }

    /**
     * @brief defines <= on vectors
     * 
     * @param vector
     * @return true if the vector is shorter or equal
     * @return false if the vector is longer
     */
    bool operator<=(Vec vector)
    {
        return (*this).norm() <= vector.norm();
    }

    /**
     * @brief defines < on vectors
     * 
     * @param vector 
     * @return true if the vector is shorter
     * @return false if the vector is longer or equal
     */
    bool operator<(Vec vector)
    {
        return (*this).norm() < vector.norm();
    }

    /**
     * @brief defines >= on vectors
     * 
     * @param vector 
     * @return true if the vector is longer or equal
     * @return false if the vector is shorter
     */
    bool operator>=(Vec vector)
    {
        return (*this).norm() >= vector.norm();
    }

    /**
     * @brief defines > on vectors
     * 
     * @param vector 
     * @return true if the vector is longer
     * @return false if the vector is shorter or equal
     */
    bool operator>(Vec vector)
    {
        return (*this).norm() > vector.norm();
    }

    /**
     * @brief dot product of two vectors
     * 
     * @param vec1 
     * @param vec2 
     * @return int
     */
    friend int operator*(const Vec vec1, const Vec vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    /**
     * @brief multiplication a scalar
     * 
     * @param scalar 
     * @return Vec 
     */
    friend Vec operator*(int scalar, Vec vector)
    {
        return Vec(vector.x * scalar, vector.y * scalar);
    }

    /**
     * @brief multiplitcation with a scalar
     * 
     * @param scalar 
     * @return Vec 
     */
    friend Vec operator*(Vec vector, int scalar)
    {
        return Vec(vector.x * scalar, vector.y * scalar);
    }

    /**
     * @brief multplication and assignment
     * 
     * @param scalar 
     * @return Vec& original vector scaled by the scalar
     */
    Vec &operator*=(int scalar)
    {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    /**
     * @brief division with a scalar
     * 
     * @param scalar 
     * @return Vec 
     */
    friend Vec operator/(int scalar, Vec vector)
    {
        return Vec(vector.x / scalar, vector.y / scalar);
    }

    /**
     * @brief division with a scalar
     * 
     * @param scalar 
     * @return Vec 
     */
    friend Vec operator/(Vec vector, int scalar)
    {
        return Vec(vector.x / scalar, vector.y / scalar);
    }

    /**
     * @brief division and assignment
     * 
     * @param scalar 
     * @return Vec& original vector scaled by the scalar
     */
    Vec &operator/=(int scalar)
    {
        x /= scalar;
        y /= scalar;

        return *this;
    }

    /**
     * @brief addition and assignment
     * 
     * @param vector 
     * @return Vec&
     */
    Vec &operator+=(Vec vector)
    {
        x += vector.x;
        y += vector.y;

        return *this;
    }

    /**
     * @brief addition of two vectors
     * 
     * @param vector 
     * @return Vec sum of the two vectors
     */
    Vec operator+(Vec vector)
    {
        return Vec(x + vector.x, y + vector.y);
    }

    /**
     * @brief subtraction and assignment
     * 
     * @param vector 
     * @return Vec&
     */
    Vec &operator-=(Vec vector)
    {
        x -= vector.x;
        y -= vector.y;

        return *this;
    }

    /**
     * @brief subtraction of two vectors
     * 
     * @param vector 
     * @return subtraction sum of the two vectors
     */
    Vec operator-(Vec vector)
    {
        return Vec(x - vector.x, y - vector.y);
    }

    /**
     * @brief norm of the vector
     * 
     * @return double length of the vector
     */
    double norm();

    /**
     * @brief vector rotated by pi/2
     * 
     * @return Vec orthogonal vector
     */
    Vec orth();
};
#endif
//define a standard vector already initialised. i mean, this is not necessary and might be a bit of a waste of memory actually but it might be useful, who knows...
#ifdef EXTRN_VEC
extern Vec vec;
#else
Vec vec;
#endif