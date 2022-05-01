#ifndef ARD
#include "Arduino.h"
#endif // !ARDUINO
#ifndef PLT_MATHS
#define PLT_MATHS


    class Vector
    {
    public:
        Vector(int, int);
        Vector(Vector &&) = default;
        Vector(const Vector &) = default;
        // Operators
        // assignment
        Vector &operator=(Vector &&) = default;
        Vector &operator=(const Vector &) = default;
        // comparison
        bool operator==(Vector v) const {return v.x == x && v.y == y;}
        bool operator<=(Vector v) {return (*this).norm() <= v.norm();}
        bool operator<(Vector v) {return (*this).norm() < v.norm();}
        bool operator>=(Vector v) {return (*this).norm() >= v.norm();}
        bool operator>(Vector v) {return (*this).norm() > v.norm();}
        
        // mathematical operations
        Vector operator+(Vector vector) const {return Vector(x + vector.x, y + vector.y);}
        Vector operator-(Vector vector) const {return Vector(x - vector.x, y - vector.y);}
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
        Vector &operator*=(int scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        Vector &operator/=(int scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // overloaded mathematical operators
        friend int operator*(const Vector vec1, const Vector vec2)
        {
            return vec1.x * vec2.x + vec1.y * vec2.y;
        }

        friend Vector operator*(int scalar, Vector vector)
        {
            return Vector(vector.x * scalar, vector.y * scalar);
        }

        friend Vector operator*(Vector vector, int scalar)
        {
            return Vector(vector.x * scalar, vector.y * scalar);
        }

         friend Vector operator/(int scalar, Vector vector)
        {
            return Vector(vector.x / scalar, vector.y / scalar);
        }

        friend Vector operator/(Vector vector, int scalar)
        {
            return Vector(vector.x / scalar, vector.y / scalar);
        }



        ~Vector();

        double norm();
        Vector orth();

        int x, y;
        
    private:
    };

    Vector::Vector(int x_init = 0, int y_init = 0)
    {
        x = x_init;
        y = y_init;
    }

    Vector::~Vector(){}

    //-----------------------------------------------------------------------------

    Vector & q_bezier();
    Vector & c_bezier();

    void mandala(); //TODO: CMON I FUCKING NEED THIS FN! i was promised some maths, give it to me...

// namespace plt (plotter_maths)
typedef Vector Vec;

#endif // !PLT_MATHS
