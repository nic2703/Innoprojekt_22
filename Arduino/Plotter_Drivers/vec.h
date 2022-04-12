#include <cmath>
#include <string>
#include <iostream>

class vec
{
public:
    int x, y;

    vec(int init_x, int init_y)
    {
        x = init_x;
        y = init_y;
    }

    bool operator==(vec param)
    {
        return param.x == x && param.y == y;
    }
    bool operator<=(vec param)
    {
        return (*this).norm() <= param.norm();
    }

    bool operator>=(vec param)
    {
        return (*this).norm() >= param.norm();
    }

    bool operator>(vec param)
    {
        return (*this).norm() > param.norm();
    }

    friend int operator*(const vec vec1, const vec vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    friend vec operator*(int param, vec vec1)
    {
        return vec(vec1.x * param, vec1.y * param);
    }

    friend vec operator*(vec vec1, int param)
    {
        return vec(vec1.x * param, vec1.y * param);
    }

    vec &operator*=(int param)
    {
        x *= param;
        y *= param;

        return *this;
    }

    friend vec operator/(int param, vec vec1)
    {
        return vec(vec1.x / param, vec1.y / param);
    }

    friend vec operator/(vec vec1, int param)
    {
        return vec(vec1.x / param, vec1.y / param);
    }

    vec &operator/=(int param)
    {
        x /= param;
        y /= param;

        return *this;
    }

    vec &operator+=(vec param)
    {
        x += param.x;
        y += param.y;

        return *this;
    }

    vec operator+(vec param)
    {
        return vec(x + param.x, y + param.y);
    }

    vec &operator-=(vec param)
    {
        x -= param.x;
        y -= param.y;

        return *this;
    }

    vec operator-(vec param)
    {
        return vec(x - param.x, y - param.y);
    }

    double norm();

    vec orth();

    friend std::ostream &operator<<(std::ostream &, vec &);
};