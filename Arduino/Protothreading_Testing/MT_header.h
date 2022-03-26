#ifndef MT_H_
#define MT_H_

typedef int pin;

//Old motor header
//DOES THIS USE PROTOTHREADING? - Ask Nic

class Motor{
    private:
        pin pspd = 0, pbrk = 0, pdir = 0;
        int bitvalspd = 0;
        float speedmms = 0.0f;
        float pos = 0.0f;
        float radius = 0.0f;
    public:
        Motor(pin pspeed, pin pbreak, pin pdirection, float position = 0.0f, float r = 10.0f);
        ~Motor();
        bool movecoord(float posnew, float speed = 100.0f);
        bool movedistance(float distance, float speed = 100.0f);
        bool startwspeed(float speed = 100.0f, bool inmms = true);
        pin pinspeed() const{
            return pspd;
        }
        pin pinbreak() const{
            return pbrk;
        }
        pin pindirection() const{
            return pdir;
        }
        float position() const{
            return pos;
        }
        bool breaks(){
            digitalWrite(pbrk, HIGH);
            return true;
        }
};

class ServoMotor : public Motor{
    private:
        int ticks = 0;
        bool islifted = false;
    public:
        ServoMotor(pin pspeed, pin pbreak, pin pdirection, float pos = 0.0f, float r = 10.0f, bool islifted = false);
        ServoMotor(Motor &mt, bool islifted = false);
        ~ServoMotor();
        bool islifted() const{
            return islifted;
        }
        int ticks() const{
            return ticks;
        }
};

Motor::Motor(pin pspeed, pin pbreak, pin pdirection, float position, float r){
    pspd = pspeed;
    pbrk = pbreak;
    pdir = pdirection;
    pos = position;
    bitvalspd = 0;
    speedmms = 0.0f;
    radius = r;
}

Motor::~Motor(){}

bool Motor::movecoord(float posnew, float speed){
    speed>0?:speed=-speed;
    bitvalspd = map(speed/(2*PI*radius), 20, 60, 30, 255);
    if (bitvalspd < 30 || bitvalspd > 255){
        bitvalspd = 100.0f;
        return false;
    }
    posnew>pos?digitalWrite(pdir, HIGH):digitalWrite(pdir, LOW);
    float distance = posnew > pos ? posnew-pos:pos-posnew;

    float time = distance/(2*PI*radius*(8.0f/15.0f*bitvalspd+15));
    digitalWrite(pbrk, LOW);
    analogWrite(pspd, bitvalspd);
    delay(1000*time);

    digitalWrite(pbrk, HIGH);
    pos = posnew;
    return true;
    
    //basically, set two variables to positions and the time for the motors to run at a specific speed (NO CURVES YET)
    //the PT_header file checks these two variables sequentially and keeps the motors on for 1 second, taking away 1000ms from the time variable
    //this means that all motor move commands will be in the PT_header.h file
}

//Motor::movedirection missing here, will add -Nic

bool Motor::startwspeed(float speed, bool inmms){
    speed > 0 ? digitalWrite(pdir, HIGH):digitalWrite(pdir, LOW), speed = -speed;       //if speed is a negative value, switch direction of the motor
    if (inmms){
        bitvalspd = map(speed/(2*PI*radius), 20, 60, 30, 255);
    } else {
        bitvalspd = speed;
    }
    if (bitvalspd < 30 || bitvalspd > 255){
        bitvalspd = 100.0f;
        return false;
    }
    digitalWrite(pbrk, LOW);                //release the handbrake
    analogWrite(pspd, bitvalspd);           //start moving at the required speed
}


#endif