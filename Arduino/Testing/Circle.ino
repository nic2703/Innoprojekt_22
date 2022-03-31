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

  double xspeed, yspeed;
  int PRECISION = 10000;

  brakingProcedure(BRAKE_A, HIGH); //disengage both brakes
  brakingProcedure(BRAKE_B, HIGH);

  /*
  brakingProcedure(BRAKE_A, LOW);
  setSpeed(SPEED_A, DIR_A, LOW, 255);

  delay(1500);

  brakingProcedure(BRAKE_A, HIGH);*/
  brakingProcedure(BRAKE_A, LOW); //disengage both brakes
  brakingProcedure(BRAKE_B, LOW);

  delay(500);
  
  for (int i = 0;i<(2*PRECISION);++i){
    xspeed = 30+(255-30)*sin(i*(2*(PI/PRECISION)));
    yspeed = 30+(255-30)*cos(i*(2*(PI/PRECISION)));

    setSpeed(SPEED_B, DIR_B, (yspeed>0)?HIGH:LOW, abs(yspeed));
    setSpeed(SPEED_A, DIR_A, (xspeed>0)?HIGH:LOW, abs(xspeed));

    delay(30/PRECISION*1000);
  }

  brakingProcedure(BRAKE_A, HIGH); //engage both brakes
  brakingProcedure(BRAKE_B, HIGH);
}

void loop() {}

void setSpeed(int speedPin, int dirPin, int dir, int speed){
    digitalWrite(dirPin, dir);
    analogWrite(speedPin, speed);
}
void brakingProcedure(int pin, int state){ // sets brakes
  digitalWrite(pin, state);
}