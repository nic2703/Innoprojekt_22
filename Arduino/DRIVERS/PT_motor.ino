#include "MT2_header.h"

void setup() {
  //speedpin of motor A is pin 3
  Serial.begin(9600);
 
  /***********************************************
   *                                             *
   *   code for individual and combinerd test    *
   *                                             *
   * *********************************************/
    Plotter plotter = Plotter(3.2f, 6.5f, 1); // there exists an object
    
}

void dualMotorTest() {
    const int SPEEDBIT = 255;

    set_brakes(_BRAKE_A, LOW);
    SET_DIR(_DIR_A, HIGH);
    set_speed(_SPEED_A, SPEEDBIT); // speed motor a to 255
    delay(2000);

    set_brakes(_BRAKE_A, HIGH);

    set_brakes(_BRAKE_B, LOW);
    SET_DIR(_DIR_B, LOW);
    set_speed(_SPEED_B, SPEEDBIT);

    delay(2000);

    set_brakes(_BRAKE_B, HIGH);

    set_brakes(_BRAKE_B, LOW);
    set_brakes(_BRAKE_A, LOW);
    //direction already set
    set_speed(_SPEED_A, SPEEDBIT);
    set_speed(_SPEED_B, SPEEDBIT);
    delay(2000);

    set_brakes(_BRAKE_B, HIGH);
    set_brakes(_BRAKE_A, HIGH); 
}

void loop(){
  dualMotorTest();
};