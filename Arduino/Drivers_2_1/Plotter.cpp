#include "Plotter_H.h"


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
    bool button_not_registered = true;
    while (button_not_registered)
    {
        abort();
    }
}