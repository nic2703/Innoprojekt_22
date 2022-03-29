# define brakePin1 8
# define brakePin2 9


void setup() {

  //Moror A
  pinMode(12, OUTPUT); // direction pin 
  pinMode(brakePin2, OUTPUT); // brake pin
  //speedpin of motor A is pin 3

  //Moror B
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


    }//}

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

/*
    for (int i = 0; i < 5; i++){
    digitalWrite(13, HIGH); //set motor B to go forward
    digitalWrite(8, LOW); //release brakes for motor B
    analogWrite(11, 180); //set speed of motor B
    delay(1000); //wait a second

    digitalWrite(8, HIGH);

    delay(500); //wait

    digitalWrite(12, HIGH); //set motor A to go forward
    digitalWrite(9, LOW); //release brakes for motor A
    analogWrite(3, 180); //set speed of motor A
    delay(1000); //wait a second

    digitalWrite(9, HIGH);

    delay(500); //wait

    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    analogWrite(11, 180); //set speed of motor B
    analogWrite(3, 180); //set speed of motor A 
    delay(1000); //wait a second

    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);

    delay(500); //wait
    */ 
    // legacy code for motor test
