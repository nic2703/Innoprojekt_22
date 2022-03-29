#include "MT2_header.h"

void set_speed(pin motorPin, pin directionPin, int direction, bit_speed speed){
    digitalWrite(directionPin, direction);
    analogWrite(motorPin, speed);
}

//sets brakes
void set_brakes(pin pin, int state){ 
  digitalWrite(pin, state);
}