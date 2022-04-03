#include <MT2_header.h>

void setup() {
  //speedpin of motor A is pin 3
  Serial.begin(9600);
 
  /***********************************************
   *                                             *
   *   code for individual and combinerd test    *
   *                                             *
   * *********************************************/
    Plotter plotter = Plotter(3.2, 6.5); // there exists an object
    
}

void dualMotorTest() {
    const int SPEEDBIT = 255;

    set_brakes(_BRAKE_A, LOW);
    set_dir(12, HIGH);
    set_speed(3, 12, HIGH, SPEEDBIT); // speed motor a to 255
    delay(2000);

    set_brakes(_BRAKE_A, HIGH);

    set_brakes(_BRAKE_B, LOW);
    set_speed(11, 13, LOW, SPEEDBIT);

    delay(2000);

    set_brakes(_BRAKE_B, HIGH);

    set_brakes(_BRAKE_B, LOW);
    set_brakes(_BRAKE_A, LOW);
    set_speed(3, 12, HIGH, SPEEDBIT);
    set_speed(3, 12, HIGH, SPEEDBIT);

    delay(2000);

    set_brakes(_BRAKE_B, HIGH);
    set_brakes(_BRAKE_A, HIGH); 
}

void loop(){
  dualMotorTest();
};