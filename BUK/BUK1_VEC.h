#ifndef BUK1_VEC_
#define BUK1_VEC_
#pragma once
//this code is mainly copied from plt_maths.h

#include <Arduino.h>
const float tolerance = 0.25;

class BUKvec{
    private:
        float x;
        float y;
    public:
        BUKvec(float init_x = 0, float init_y = 0);
        ~BUKvec();
        BUKvec& operator=(const BUKvec&);
        float operator[](int i);
        bool operator==(const BUKvec vector) const;
        bool operator!=(const BUKvec vector) const;
        bool operator<=(const BUKvec vector) const;
        bool operator<(const BUKvec vector) const;
        bool operator>=(const BUKvec vector) const;
        bool operator>(const BUKvec vector) const;

        double operator*(const BUKvec vector) const;
        BUKvec operator*(const int scalar) const;
        friend BUKvec operator*(int scalar, BUKvec vector){
            return vector*scalar;
        }

        BUKvec operator/(const int divisor) const;
        friend float operator/(const int numerator, const BUKvec vector){
            return numerator/vector.norm();
        }

        BUKvec operator+(const BUKvec vector) const;
        void operator+=(const BUKvec vector);
        BUKvec operator-(const BUKvec vector) const;
        void operator-=(const BUKvec vector);

        double norm() const;
};
#endif