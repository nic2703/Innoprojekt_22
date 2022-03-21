void setup() {
  //Moror A
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT);

  //Moror B
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);

  for (int i=0; i<5; ++i){
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW); 
    analogWrite(11, 80);
    delay(1000);

    digitalWrite(8, HIGH);

    delay(500);
    
    digitalWrite(13, LOW);
    digitalWrite(8, LOW); 
    analogWrite(11, 80);
    delay(1000);

    digitalWrite(8, HIGH);
    
    delay(500);
  }
}

void loop(){}
