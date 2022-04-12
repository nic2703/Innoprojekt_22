#include "vec.h"

double vec::norm()
{
    return sqrt(sq(*this));
}

vec vec::orth()
{
    return vec(this->y, -(this->x));
}