#include "vec.h"

double Vec::norm()
{
    return sqrt(sq(*this));
}

vec Vec::orth()
{
    return vec(this->y, -(this->x));
}