#include "plt_maths.h"
typedef plt::Vector Vec;

double Vec::norm()
{
    return sqrt(sq(*this));
}

Vec Vec::orth()
{
    return Vec(this->y, -(this->x));
}