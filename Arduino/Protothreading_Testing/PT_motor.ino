# define brakePin1 8
# define brakePin2 9

#include "MT2_header.h"

void setup() {

  //Mordor A
  pinMode(12, OUTPUT); // direction pin 
  pinMode(brakePin2, OUTPUT); // brake pin
  //speedpin of motor A is pin 3

  //Mordor B
  pinMode(13, OUTPUT); // direction pin
  pinMode(brakePin1, OUTPUT); // brake pin
  //speedpin of motor B is 11
  
  breakingProcedure(brakePin1, HIGH); //engage both brakes
  breakingProcedure(brakePin2, HIGH); 

    /***********************************************
     *                                             *
     *   code for individual and combinerd test    *
     *                                             *
     * *********************************************/
    Plotter plotter = Plotter(3.2, 6.5); // there exists an object

    }

void dualMotorTest() {
    const int SPEEDBIT = 255;

    breakingProcedure(brakePin2, LOW);
    setSpeed(3, 12, HIGH, SPEEDBIT); // speed motor a to 180
    delay(2000);

    breakingProcedure(brakePin2, HIGH);

    breakingProcedure(brakePin1, LOW);
    setSpeed(11, 13, LOW, SPEEDBIT);

    delay(2000);

    breakingProcedure(brakePin1, HIGH);

    breakingProcedure(brakePin1, LOW);
    breakingProcedure(brakePin2, LOW);
    setSpeed(3, 12, HIGH, SPEEDBIT);
    setSpeed(3, 12, HIGH, SPEEDBIT);

    delay(2000);

    breakingProcedure(brakePin1, HIGH);
    breakingProcedure(brakePin2, HIGH); 
}

void setSpeed(int motorPin, int directionPin, int direction, int speed){
    digitalWrite(directionPin, direction);
    analogWrite(motorPin, speed);
}
void breakingProcedure(int pin, int state){ // sets brakes
  digitalWrite(pin, state);
}

void loop(){
  dualMotorTest();
};
