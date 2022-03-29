#include <MT2_header.h>

# define brakePin1 8
# define brakePin2 9

void setup() {

  //Mordor A
  pinMode(12, OUTPUT); // direction pin 
  pinMode(brakePin2, OUTPUT); // brake pin
  //speedpin of motor A is pin 3
  Serial.begin(9600);

  //Mordor B
  pinMode(13, OUTPUT); // direction pin
  pinMode(brakePin1, OUTPUT); // brake pin
  //speedpin of motor B is 11
  
  set_brakes(brakePin1, HIGH); //engage both brakes
  set_brakes(brakePin2, HIGH); 

    /***********************************************
     *                                             *
     *   code for individual and combinerd test    *
     *                                             *
     * *********************************************/
    Plotter plotter = Plotter(3.2, 6.5); // there exists an object

    }

void dualMotorTest() {
    const int SPEEDBIT = 255;

    set_brakes(brakePin2, LOW);
    set_speed(3, 12, HIGH, SPEEDBIT); // speed motor a to 180
    delay(2000);

    set_brakes(brakePin2, HIGH);

    set_brakes(brakePin1, LOW);
    set_speed(11, 13, LOW, SPEEDBIT);

    delay(2000);

    set_brakes(brakePin1, HIGH);

    set_brakes(brakePin1, LOW);
    set_brakes(brakePin2, LOW);
    set_speed(3, 12, HIGH, SPEEDBIT);
    set_speed(3, 12, HIGH, SPEEDBIT);

    delay(2000);

    set_brakes(brakePin1, HIGH);
    set_brakes(brakePin2, HIGH); 
}

void loop(){
  dualMotorTest();
};