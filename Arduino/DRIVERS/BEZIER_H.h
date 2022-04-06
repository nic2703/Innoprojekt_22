#ifndef BEZIER_H
#define BEZIER_H

#include <Arduino.h>
#include "MT2_header.h"

/* unint32_t ! struct works on assumption that the paper is a fine grid. one unit of translation is equivalent to 
0.01 mm //TODO: discuss this 
*/
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

    bool robotica_test(int = 1);
    bool circle_segment_offr(double, double, double, uint32_t, uint32_t);
    bool circle_segment_onr(double, double, double);
    bool random_curve();
};

#endif // !BEZIER_H