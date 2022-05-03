#ifndef PMTH
#define PMTH

#define cube(x) ((x) * (x) * (x))                                                                                                           // cubes x
#define speed_to_bits(s) (((s) == 0) ? 0 : (28.97 + 3.402 * (abs(s) * 58.5) - 0.1736 * sq(abs(s) * 58.5) + 0.003101 * cube(abs(s) * 58.5))) // caluculates bit value needed for bits, takes float returns float in the range [0, 1] ||| (0,1] --> [30, 255] + 0 --> 0
#define bits_to_speed(s) (1.012e-5 * cube(s) - 6.19e-3 * sq(s) + 1.332 * s - 37.24)                                                         // bytespeed to actual irl speed
#define set_dir(p_dir, s) digitalWrite(p_dir, (((s) > 0) ? HIGH : LOW))                                                                     // if delta is negative, go backwards, else go forwards

namespace pmath
{
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

        double norm() const;

        Vector orth() const;

    private:
        long x, y;
    };


    long qbez_x(long, long, long, uint8_t, uint8_t);
    long qbez_x(long, long, long, uint8_t, uint8_t);
    long cbez_x(long, long, long, long, uint8_t, uint8_t);
    long cbez_y(long, long, long, long, uint8_t, uint8_t);
}

typedef pmath::Vector Vec;

#endif // !PMTH