/**
 * @file pmath.h
 * @author Robotica (vdamani@ethz.ch)
 * @brief 
 * @version 2.0
 * @date 2022-05-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PMTH
#define PMTH

#define C 0.5519150244935105707435627 
// source: https://spencermortensen.com/articles/bezier-circle/

#define cube(x) ((x)*(x)*(x))                                                                                                           // cubes x
#define speed_to_bits(s) (((s) == 0) ? 0 : (28.97 + 3.402 * (abs(s) * 58.5) - 0.1736 * sq(abs(s) * 58.5) + 0.003101 * cube(abs(s) * 58.5))) // caluculates bit value needed for bits, takes float returns float in the range [0, 1] ||| (0,1] --> [30, 255] + 0 --> 0
#define bits_to_speed(s) (1.012e-5 * cube(s) - 6.19e-3 * sq(s) + 1.332 * s - 37.24)                                                         // bytespeed to actual irl speed

#define sign(x) (((x)>0)-((x)<0)) //nice to have, return values {-1, 0, 1}
#undef abs
#define abs(x) ((x)*sign((x))) //better 

namespace pmath
{
    template<typename T>
    class Vector
    {
    public:
        Vector(int x_init = 0, int y_init = 0)
        {
            x = x_init;
            y = y_init;
        }

        Vector(Vector &&) = default;
        Vector(const Vector &) = default;

        Vector &operator=(Vector &&) = default;
        Vector &operator=(const Vector &) = default;

        int _x() const { return x; }
        int _y() const { return y; }

        long & operator[](int i)
        {
            switch (i)
            {
                case 0: return this->x; 
                case 1: return this->y;
        }   }
        long operator[](int i) /*[[expects: (i >= 0 && i < 2)]]*/ const {return x * !i + y * i;}


        bool operator==(Vector v) const { return v.x == x && v.y == y; }
        bool operator!=(Vector v) const { return v.x != x && v.y != y; }
        bool operator<=(Vector v) { return (*this).norm() <= v.norm(); }
        bool operator<(Vector v) { return (*this).norm() < v.norm(); }
        bool operator>=(Vector v) { return (*this).norm() >= v.norm(); }
        bool operator>(Vector v) { return (*this).norm() > v.norm(); }

        Vector operator+(Vector vector) const { return Vector(x + vector.x, y + vector.y); }
        Vector operator-(Vector vector) const { return Vector(x - vector.x, y - vector.y); }

        Vector &operator+=(Vector vector)
        {
            x += vector.x;
            y += vector.y;
            return *this;
        }
        Vector &operator-=(Vector vector)
        {
            x -= vector.x;
            y -= vector.y;

            return *this;
        }
        Vector &operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        Vector &operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        friend Vector operator*(T scalar, Vector vector)
        {
            return Vector(vector.x * scalar, vector.y * scalar);
        }

        friend Vector operator*(Vector vector, T scalar)
        {
            return Vector(vector.x * scalar, vector.y * scalar);
        }

        friend Vector operator/(T scalar, Vector vector)
        {
            return Vector(vector.x / scalar, vector.y / scalar);
        }

        friend Vector operator/(Vector vector, T scalar)
        {
            return Vector(vector.x / scalar, vector.y / scalar);
        }

        double norm() const;

        Vector orth() const;

        Vector rotate(double);
        Vector post_rotate(double);

    private:
        T x, y;
    };


    long qbez_x(long, long, long, uint8_t, uint8_t);
    long qbez_x(long, long, long, uint8_t, uint8_t);
    long cbez_x(long, long, long, long, uint8_t, uint8_t);
    long cbez_y(long, long, long, long, uint8_t, uint8_t);

}

typedef pmath::Vector<int> Vec;
typedef pmath::Vector<double> Vec_d;

#endif // !PMTH