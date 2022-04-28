#include "math.h"
#include "BUK1_VEC.h"

BUKvec::BUKvec(float init_x, float init_y){
    x = init_x;
    y = init_y;
}

BUKvec::~BUKvec(){};

BUKvec& BUKvec::operator=(const BUKvec& vector){
    if (this == &vector){
        return *this;
    }
    x = vector.x;
    y = vector.y;
    return *this;
}

float BUKvec::operator[](int i){
    float error = -1.0f;
    switch(i){
        case 0: return x;
        case 1: return y;
        default: return error;
    }
}

bool BUKvec::operator==(const BUKvec vector) const{
    if (abs(x-vector.x)<=tolerance && abs(y-vector.y)<=tolerance){
        return true;
    }
    return false;
}

bool BUKvec::operator!=(const BUKvec vector) const{
    return !this->operator==(vector);
}

bool BUKvec::operator<=(const BUKvec vector) const{
    if ((x<vector.x && y<vector.y) || (*this==vector)){
        return true;
    }
    return false;
}

bool BUKvec::operator<(const BUKvec vector) const{
    if ((x<vector.x && y<vector.y) && (*this!=vector)){
        return true;
    }
    return false;
}

bool BUKvec::operator>=(const BUKvec vector) const{
    if ((x>vector.x && y>vector.y) || (*this==vector)){
        return true;
    }
    return false;
}

bool BUKvec::operator>(const BUKvec vector) const{
    if ((x>vector.x && y>vector.y) && (*this!=vector)){
        return true;
    }
    return false;
}

double BUKvec::operator*(const BUKvec vector) const{
    double result = x*vector.x+y*vector.y;
    return result;
}

BUKvec BUKvec::operator*(const int scalar) const{
    return BUKvec(scalar*x, scalar*y);
}

BUKvec BUKvec::operator/(const int divisor) const{
    return BUKvec(x/divisor, y/divisor);
}

BUKvec BUKvec::operator+(const BUKvec vector) const{
    return BUKvec(x+vector.x, y+vector.y);
}

void BUKvec::operator+=(const BUKvec vector){
    x+=vector.x;
    y+=vector.y;
}

BUKvec BUKvec::operator-(const BUKvec vector) const{
    return BUKvec(x-vector.x, y-vector.y);
}

void BUKvec::operator-=(const BUKvec vector){
    x-=vector.x;
    y-=vector.y;
}

double BUKvec::norm() const{
    //return sqrt(this[0]*this[0]+this[1]*this[1]);
    return sqrt(x*x+y*y);
}