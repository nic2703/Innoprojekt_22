#ifndef PLT_MATH_H
#define PLT_MATH_H

#include <Arduino.h>

struct coords //FIXME: find a better name for this pls
{
    int x;
    int y;
    
    /**
     * @brief defines addition on vectors
     * 
     * @param param vector to add
     * @return coords sum of the two vectors
     */
    coords operator+(coords &param)
    {
        x += param.x;
        y += param.y;

        return *this;
    }

    coords& operator+=(coords &param)
    {
        this->x += param.x;
        this->y += param.y;

        return *this;
    }

    /**
     * @brief defines subtraction on vectors
     * 
     * @param param vector to subtract
     * @return coords difference of the two vectors
     */
    coords operator-(coords &param)
    {
        x -= param.x;
        y -= param.y;

        return *this;
    }

    coords& operator-=(coords &param)
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
    int operator*(coords &param)
    {
        return x*param.x + y*param.y;
    }

    coords operator*(int &param)
    {
        x *= param;
        y *= param;

        return *this;
    }

    coords& operator*=(int &param)
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
    bool operator==(coords &param)
    {
        return (x == param.x && y == param.y);
    }

    /**
     * @brief 
     * 
     * @return coords of a vector perpendicular to this one
     */
    coords normal()
    {
        int t = x;
        x = y;
        y = -t;

        return *this;
    }

};

/**
 * @brief calculates the norm of a vector
 * @param vec vector of type coords
 * @return length of the vector 
 */
double norm();


/**
 * @brief Calculates scalar product
 * @param vec1 @param vec2
 * @returns scalar product of vec1 with vec2
 */
inline int scalar_product(coords, coords);

#endif