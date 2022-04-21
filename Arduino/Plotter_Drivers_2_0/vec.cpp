#include "vec.h"

double Vec::norm()
{
    return sqrt(sq(*this));
}

Vec Vec::orth()
{
    return Vec(this->y, -(this->x));
}