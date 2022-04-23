#include "Plotter_H.h"

#ifndef PLT
    #error Plotter not defined
#endif // !PLT


inline void set_speed(pin, int)
{
}

inline void set_brakes(pin, int)
{
}

[[maybe_unused]] void _init_servo()
{
    servo.attach(_SERVO);
    Serial.println("attached servo to pin 4");
}

Plt::Plt()
{
    __plt_init();
}

Plt::~Plt()
{
}

bool Plt::__plt_init()
{
    // run into walls till button registered
    x_speed = _SPEED_A, x_dir = _DIR_A, x_brk = _BRAKE_A;
    y_speed = _SPEED_B, y_dir = _DIR_B, y_brk = _BRAKE_B;

    uint16_t time_A = micros();

    bool button_not_registered = true;
    while (button_not_registered)
    {
        
    }
}