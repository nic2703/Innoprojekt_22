#include "BEZIER_H.h"

void setup() {
  //speedpin of motor A is pin 3
  Serial.begin(9600);

    int input; //0 to 1023
    BEZIER plotter = BEZIER(); // there exists an object
    
    for (uint32_t i = 0; i < 3; i++)
    {
      input = Serial.read();
      plotter.set_servo(input);  
    }
}

void loop(){
};