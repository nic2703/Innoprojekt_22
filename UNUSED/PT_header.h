#ifndef PT_H_
#define PT_H_
#include "TimedAction.h"
#include <HardwareSerial.h>

bool moveXfor(int ms);
bool moveYfor(int ms);

unsigned long int timer = 0;
//things to be protothreaded are initialized as TimedAction objects
TimedAction MXAction = TimedAction(1000, MotorX);       //every 1 second check
TimedAction MYAction = TimedAction(1000, MotorY);

void setup(){
    Serial.begin(9600);
    moveXfor(3000);
    
}

void loop(){
    Serial.println("Iteration #");
    Serial.print(timer);
    Serial.print("  ");
    Serial.print(millis());
    Serial.print("ms");
    MXAction.check();
    MYAction.check();
}

void MotorX(){
    
}

void MotorY(){
    
}







#endif