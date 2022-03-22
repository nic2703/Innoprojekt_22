void setup() {

  //Moror A
  pinMode(12, OUTPUT); // direction pin 
  pinMode(9, OUTPUT); // brake pin
  //speedpin of motor A is pin 3

  //Moror B
  pinMode(13, OUTPUT); // direction pin
  pinMode(8, OUTPUT); // brake pin
  
  digitalWrite(9, HIGH); //engage both brakes
  digitalWrite(8, LOW); 

  for (int i=0; i<5; ++i){
    digitalWrite(13, HIGH); //set motor B to go forward
    digitalWrite(8, LOW); //release brakes for motor B
    analogWrite(11, 80); //set speed of motor B
    delay(1000); //wait a second

    digitalWrite(8, HIGH);

    delay(500); //wait
    
    digitalWrite(13, LOW);
    digitalWrite(8, LOW); 
    analogWrite(11, 80);
    delay(1000); //wait

    digitalWrite(8, HIGH); //engage brake
    
    delay(500); //wait
  }
}

void loop(){}
