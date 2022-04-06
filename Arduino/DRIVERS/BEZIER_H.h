#ifndef BEZIER_H
#define BEZIER_H

#include <Arduino.h>
#include "MT2_header.h"

struct COORDS
{
    uint32_t x;
    uint32_t y;
} curve = {0,0};

class BEZIER : public Plotter
{
private:
    void _init_coords();
public:
    BEZIER(/* args */);
    ~BEZIER();

    bool robotica_test();
    bool circle_segment(double, double, double, int);
    bool random_curve();
};

#endif // !BEZIER_H