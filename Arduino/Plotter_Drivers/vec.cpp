#include "vec.h"

double vec::norm()
{
    return std::sqrt((*this) * (*this));
}

vec vec::orth()
{
    return vec(this->y, -(this->x));
}

std::ostream &operator<<(std::ostream &os, vec &param)
{
    os << "{" << param.x << ", " << param.y << "}";
    return os;
}
