#include "Plotter_H.h"

#ifndef PLT
#error Plotter not defined
#endif // !PLT

inline void set_speed(pin motor_a, int speed_a)
{
    analogWrite(motor_a, speed_a);
}

inline void set_brakes(pin motor, int state)
{
    digitalWrite(motor, state);
}

[[maybe_unused]] void _init_servo()
{
    servo.attach(_SERVO);
    Serial.println("attached servo to pin 4");
}

Plt::Plt()
{
    __plt_init(1);
}

Plt::~Plt() {}

bool Plt::__plt_init(pin button)
{
    // run into walls till button registered
    x_speed = _SPEED_A, x_dir = _DIR_A, x_brk = _BRAKE_A;
    y_speed = _SPEED_B, y_dir = _DIR_B, y_brk = _BRAKE_B;

    pin buttonA = button;
    pinMode(buttonA, INPUT);

    uint8_t button_state;
    uint8_t last_button_state = LOW, debounce = 50;
    uint16_t timeA;

    bool button_not_registered = true;
    uint16_t start = millis();
    while (button_not_registered)
    {
        int readingA = digitalRead(buttonA);
        if (readingA != last_button_state)
        {
            timeA = millis();
        }

        if (millis() - timeA > debounce)
        {
            if (button_state != last_button_state)
            {
                button_state = readingA;

                if (button_state == HIGH)
                {
                    timeA = millis() - start;
                    button_not_registered = false;
                }
            }
        }
    }
}