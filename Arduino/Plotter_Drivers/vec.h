#include <cmath>
#include <string>
#include <iostream>

class vec
{
public:
    int x; //xcoordinate
    int y; //y coordinate

    /**
     * @brief Construct a new vec object
     * 
     * @param init_x x coordinate
     * @param init_y y coordinate
     */
    vec(int init_x, int init_y)
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
    bool operator==(vec vector)
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
    bool operator<=(vec vector)
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
    bool operator<(vec vector)
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
    bool operator>=(vec vector)
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
    bool operator>(vec vector)
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
    friend int operator*(const vec vec1, const vec vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    /**
     * @brief multiplication a scalar
     * 
     * @param scalar 
     * @return vec 
     */
    friend vec operator*(int scalar, vec vector)
    {
        return vec(vector.x * scalar, vector.y * scalar);
    }

    /**
     * @brief multiplitcation with a scalar
     * 
     * @param scalar 
     * @return vec 
     */
    friend vec operator*(vec vector, int scalar)
    {
        return vec(vector.x * scalar, vector.y * scalar);
    }

    /**
     * @brief multplication and assignment
     * 
     * @param scalar 
     * @return vec& original vector scaled by the scalar
     */
    vec &operator*=(int scalar)
    {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    /**
     * @brief division with a scalar
     * 
     * @param scalar 
     * @return vec 
     */
    friend vec operator/(int scalar, vec vector)
    {
        return vec(vector.x / scalar, vector.y / scalar);
    }

    /**
     * @brief division with a scalar
     * 
     * @param scalar 
     * @return vec 
     */
    friend vec operator/(vec vector, int scalar)
    {
        return vec(vector.x / scalar, vector.y / scalar);
    }

    /**
     * @brief division and assignment
     * 
     * @param scalar 
     * @return vec& original vector scaled by the scalar
     */
    vec &operator/=(int scalar)
    {
        x /= scalar;
        y /= scalar;

        return *this;
    }

    /**
     * @brief addition and assignment
     * 
     * @param vector 
     * @return vec&
     */
    vec &operator+=(vec vector)
    {
        x += vector.x;
        y += vector.y;

        return *this;
    }

    /**
     * @brief addition of two vectors
     * 
     * @param vector 
     * @return vec sum of the two vectors
     */
    vec operator+(vec vector)
    {
        return vec(x + vector.x, y + vector.y);
    }

    /**
     * @brief subtraction and assignment
     * 
     * @param vector 
     * @return vec&
     */
    vec &operator-=(vec vector)
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
    vec operator-(vec vector)
    {
        return vec(x - vector.x, y - vector.y);
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
     * @return vec orthogonal vector
     */
    vec orth();

    /**
     * @brief defines the << operator for the ostream
     * 
     * @return std::ostream& 
     */
    friend std::ostream &operator<<(std::ostream &, vec &);
};