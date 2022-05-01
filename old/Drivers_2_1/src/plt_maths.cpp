#include "plt_maths.h"


double Vec::norm()
{
    return sqrt(sq(*this));
}

Vec Vec::orth()
{
    return Vec(this->y, -(this->x));
}

/* Vec & c_bezier(Vec & position, Vec & control1, Vec & control2, Vec & coords, unsigned int prec, unsigned int i){
    float t = i/prec;
    float x = cube(1-t)*position.x + sq(1-t)*3*t*control1.x + (1-t)*3*sq(t)*control2.x + cube(t)*coords.x;
    float y = cube(1-t)*position.y + sq(1-t)*3*t*control1.y + (1-t)*3*sq(t)*control2.y + cube(t)*coords.y;
    Vec subpoint(x, y);
    return subpoint;
}

Vec & q_bezier(Vec & position, Vec & control1, Vec & coords, unsigned int prec, unsigned int i){
    float t = i/prec;
    float x = sq(1-t)*position.x + (1-t)*2*t*control1.x + sq(t)*coords.x;
    float y = sq(1-t)*position.y + (1-t)*2*t*control1.y + sq(t)*coords.y;
    Vec subpoint(x, y);
    return subpoint;
} */