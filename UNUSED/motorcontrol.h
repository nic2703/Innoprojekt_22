#ifndef MOTORC_H_
#define MOTORC_H_
#include <time.h>

class MotorC{
    private:
        int pdir, pbrk, pspd;
        unsigned int speedval = 0;
        float radius;
        float position;                                      //holds the position relative to motor runtime and speed
    public:
        MotorC(int pd, int pb, int ps, float pos, float r);                     //initialize to a position (ex. 20mm from edge)
        ~MotorC();
        bool motorcoord(float posnew);                  //move motor to a coordinate (mm) base speed 80 ~ 22 rpm
        bool motorcoord(float posnew, float time);      //move motor to a coordinate in a time (s)
        bool motormove(float ticks);                    //move motor x units (mm)
        bool motormove(float ticks, float time);        //move motor x units in a time (s)
        float motorcoord() const{
            return position;
        };
        float setspeed(int bitval) {
            speedval = bitval;
        }
};

class ServoMotor : public MotorC{                       //Derived class for servo
    private:
        float zero;                                     //Zero value for servo/stepper/encoder
        bool islifted;                                  //boolean specifically for pen lifting
    public:
        ServoMotor(int pin, float pos, float zero);
        ~ServoMotor();
        bool islifted() const{
            return islifted;
        }
        float zero() const{
            return zero;
        }
};

//Following for testing purposes on PC:

MotorC::MotorC(int pdirection, int pbreak, int pspeed, float pos, float r){
    pdir = pdirection;
    pbrk = pbreak;
    pspd = pspeed; 
    position = pos;
    radius = r;                                         //pls in mm
}

MotorC::~MotorC(){};

bool MotorC::motorcoord(float posnew){
    setspeed(100);
    posnew>position?digitalWrite(pdir, HIGH):digitalWrite(pdir, LOW);
    float distance = posnew > position ? posnew-position:position-posnew;
    float time = distance/(2*PI*radius*(8.0f/15.0f*speedval+15));
    digitalWrite(pbrk, LOW);
    analogWrite(pspd, speedval);
    delay(1000*time);
    digitalWrite(pbrk, HIGH);
    position = posnew;
    return true;
}

bool MotorC::motorcoord(float posnew, float time){
    posnew>position?digitalWrite(pdir, HIGH):digitalWrite(pdir, LOW);
    float distance = posnew > position ? posnew-position:position-posnew;
    if (distance/(60*radius*PI)>time){
        return false;
    } else {
        setspeed(45.0f/8.0f*(distance/(2*PI*radius*time)-15));
    }
    digitalWrite(pbrk, LOW);
    analogWrite(pspd, speedval);
    delay(1000*time);
    digitalWrite(pbrk, HIGH);
    position = posnew;
    return true;    
}
#endif