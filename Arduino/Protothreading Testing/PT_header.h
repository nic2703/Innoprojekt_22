#ifndef PT_H_
#define PT_H_
#include "TimedAction.h"

//things to be protothreaded are initialized as TimedAction objects

TimedAction MXAction = TimedAction(1000, MotorX);       //every 1 second
TimedAction MYAction = TimedAction(1000, MotorY);

void loop(){
    MXAction.check();
    MYAction.check();
}

void MotorX(){
    
}







#endif