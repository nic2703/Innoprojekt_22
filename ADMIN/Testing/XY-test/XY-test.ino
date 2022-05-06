#define BRAKE_A 9
#define BRAKE_B 8
#define SPEED_A 3
#define SPEED_B 11
#define DIR_A 12
#define DIR_B 13

void setup() {

  //Moror A
  pinMode(DIR_A, OUTPUT); // direction pin 
  pinMode(BRAKE_A, OUTPUT); // brake pin
  //speedpin of motor A is pin 3

  //Moror B
  pinMode(DIR_B, OUTPUT); // direction pin
  pinMode(BRAKE_B, OUTPUT); // brake pin
  //speedpin of motor B is 11
  
  brakingProcedure(BRAKE_A, HIGH); //engage both brakes
  brakingProcedure(BRAKE_B, HIGH); 

  for (int i=0;i<5;++i){
      setSpeed(SPEED_A, DIR_A, HIGH, 100);
      brakingProcedure(BRAKE_A, LOW);
      delay(500);

      brakingProcedure(BRAKE_A, HIGH);
      delay(500);

      setSpeed(SPEED_A, DIR_A, LOW, 100);
      brakingProcedure(BRAKE_A, LOW);
      delay(500);

      brakingProcedure(BRAKE_A, HIGH);
      delay(500);

      setSpeed(SPEED_B, DIR_B, HIGH, 100);
      brakingProcedure(BRAKE_B, LOW);
      delay(500);

      brakingProcedure(BRAKE_B, HIGH);
      delay(500);

      setSpeed(SPEED_B, DIR_B, HIGH, 100);
      brakingProcedure(BRAKE_B, LOW);
      delay(500);

      brakingProcedure(BRAKE_B, HIGH);
      delay(500);
  }

}

void setSpeed(int speedPin, int dirPin, int dir, int speed){
    digitalWrite(dirPin, dir);
    analogWrite(speedPin, speed);
}
void brakingProcedure(int pin, int state){ // sets brakes
  digitalWrite(pin, state);
}

void loop(){}