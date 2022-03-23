#ifndef MT2_H_
#define MT2_H_
#include <HardwareSerial.h>
typedef unsigned int pin;
float MINDIST = 1.0f;               //minimum distance the motor can turn in mm

class Plotter{
    private:
        float xpos = 0.0f, ypos = 0.0f;
        pin xpspd = 0, xpbrk = 0, xpdir = 0;
        pin ypspd = 0, ypbrk = 0, ypdir = 0;
        pin zpspd = 0, zpbrk = 0, zpdir = 0;
        bool islifted = false;
        float radiusx = 0.0f, radiusy = 0.0f;

        bool straightlinex(float xdelta){
            xdelta>0?digitalWrite(xpdir, HIGH):digitalWrite(xpdir, LOW);
            float duedate = millis();
            float ms = 1000.0f*(xdelta)/(2*PI*radiusx);
            duedate += ms;
            digitalWrite(xpbrk, LOW);                //release the handbrake
            analogWrite(xpspd, 255);
            float currenttime = millis();
            while (millis()<duedate){
                //welcome to the waiting line
                if (millis() > currenttime + 10000){
                    Serial.println("Overran 10 second limit for straight line X move!");
                    digitalWrite(xpbrk, HIGH);
                    return false;
                    break;
                }
            }
            digitalWrite(xpbrk, HIGH);
            Serial.println("Straight line X move complete!");
            return true;
        }

        bool straightliney(float ydelta){
            ydelta>0?digitalWrite(ypdir, HIGH):digitalWrite(ypdir, LOW);
            float duedate = millis();
            float ms = 1000.0f*(ydelta)/(2*PI*radiusy);
            duedate += ms;
            digitalWrite(ypbrk, LOW);                //release the handbrake
            analogWrite(ypspd, 255);
            float currenttime = millis();
            while (millis()<duedate){
                //welcome to the waiting line
                if (millis() > currenttime + 10000){
                    Serial.println("Overran 10 second limit for straight line Y move!");
                    digitalWrite(ypbrk, HIGH);
                    return false;
                    break;
                }
            }
            digitalWrite(ypbrk, HIGH);
            Serial.println("Straight line Y move complete!");
            return true;
        }

        bool normalline(float xdelta, float ydelta){
            //higher delta always has the maximum speed of 255
            xdelta>0?digitalWrite(xpdir, HIGH):digitalWrite(xpdir, LOW);
            ydelta>0?digitalWrite(ypdir, HIGH):digitalWrite(ypdir, LOW);
            float duedate = millis();
            if (abs(xdelta) > abs(ydelta)){
                float ms = 1000.0f*(xdelta)/(2*PI*radiusx);
                duedate += ms;
                digitalWrite(xpbrk, LOW);                //release the handbrake
                analogWrite(xpspd, 255);
                digitalWrite(ypbrk, LOW);                //release the handbrake
                analogWrite(ypspd, (ydelta/xdelta)*255.0f);
            } else {
                float ms = (ydelta)/(2*PI*radiusy);
                duedate += ms;
                digitalWrite(ypbrk, LOW);                //release the handbrake
                analogWrite(ypspd, 255);
                digitalWrite(xpbrk, LOW);                //release the handbrake
                analogWrite(xpspd, (xdelta/ydelta)*255.0f);
            }
            float currenttime = millis();
            while (millis()<duedate){
                //welcome to the waiting line
                if (millis() > currenttime + 10000){
                    Serial.println("Overran 10 second limit for normal line move!");
                    digitalWrite(xpbrk, HIGH);
                    digitalWrite(ypbrk, HIGH);
                    return false;
                    break;
                }
            }
            digitalWrite(xpbrk, HIGH);
            digitalWrite(ypbrk, HIGH);
            Serial.println("Normal line move complete!");
            return true;
        }

        bool specialline(float xdelta, float ydelta){
            xdelta>0?digitalWrite(xpdir, HIGH):digitalWrite(xpdir, LOW);
            ydelta>0?digitalWrite(ypdir, HIGH):digitalWrite(ypdir, LOW);
            if (abs(xdelta) > abs(ydelta)){
                float msx  = 1000.0f*(xdelta*(MINDIST/(2.0f*ydelta)))/(2*PI*radiusx);             //distance in order to achieve a 1mm rise through the theoretical perfect rise
                if (msx > xdelta){
                    straightlinex(xdelta);
                    straightliney(ydelta);      //technically smaller than the MINDIST value
                } else {
                    //here comes the incredibly difficult zigzag pattern
                    int numiterations = xdelta/msx;
                    for (int i = 0; i  < numiterations; i++){
                        straightlinex(msx);
                        straightliney(MINDIST);
                    }
                    straightlinex(xdelta-numiterations*msx);
                    straightliney(ydelta-numiterations*MINDIST);
                }
            } else {
                float msy  = 1000.0f*(ydelta*(MINDIST/(2.0f*xdelta)))/(2*PI*radiusy);             //distance in order to achieve a 1mm rise through the theoretical perfect rise
                if (msy > xdelta){
                    straightliney(ydelta);
                    straightlinex(xdelta);      //technically smaller than the MINDIST value
                } else {
                    //here comes the incredibly difficult zigzag pattern
                    int numiterations = ydelta/msy;
                    for (int i = 0; i  < numiterations; i++){
                        straightliney(msy);
                        straightlinex(MINDIST);
                    }
                    straightliney(ydelta-numiterations*msy);
                    straightlinex(xdelta-numiterations*MINDIST);
                }

            }
        }

    public:
        Plotter(float xposition = 0.0f, float yposition = 0.0f);
        ~Plotter();
        bool setpinX(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
        bool setpinY(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
        bool setpinZ(pin pinspeed = 0, pin pinbreak = 0, pin pindirection = 0);
        //set radii_cogwheels
        bool resetpos(float xposition, float yposition);
        bool moveline(float posx = 0.0f, float posy = 0.0f, float speed = 100);
        //implement lift pen
        //implement set pen

};

Plotter::Plotter(float xposition, float yposition){
    xpos = xposition;
    ypos = yposition;
}

Plotter::~Plotter(){};

bool Plotter::setpinX(pin pinspeed, pin pinbreak, pin pindirection){
    if (pinspeed > 30 || pinbreak > 30 || pindirection > 30){
        return false;
    }
    xpspd = pinspeed;
    xpbrk = pinbreak;
    xpdir = pindirection;
    return true;
}

bool Plotter::setpinY(pin pinspeed, pin pinbreak, pin pindirection){
    if (pinspeed > 30 || pinbreak > 30 || pindirection > 30){
        return false;
    }
    ypspd = pinspeed;
    ypbrk = pinbreak;
    ypdir = pindirection;
    return true;
}

bool Plotter::setpinZ(pin pinspeed, pin pinbreak, pin pindirection){
    if (pinspeed > 30 || pinbreak > 30 || pindirection > 30){
        return false;
    }
    zpspd = pinspeed;
    zpbrk = pinbreak;
    zpdir = pindirection;
    return true;
}

bool Plotter::resetpos(float xposition, float yposition){
    xpos = xposition;
    ypos = yposition;
    //can add condition for coords to be in plane later
    return true;
}

bool Plotter::moveline(float xposnew, float yposnew, float speed){
    float xdelta = xposnew - xpos;
    float ydelta = yposnew - ypos;
    if (abs(xdelta) <= MINDIST){
        straightliney(ydelta);
    } else if (abs(ydelta) <= MINDIST){
        straightlinex(xdelta);
    } else if (abs(ydelta/xdelta) < 1.0f/3.0f || abs(ydelta/xdelta) > 3.0f){
        //use normal line
        if(normalline(xdelta, ydelta)){
            xpos += xdelta;
            ypos += ydelta;
            return true;
        } else {
            Serial.println("Encoder error, x-y-positions may be incorrect");
            return false;
        }
    } else {
        //use special line
        if(specialline(xdelta, ydelta)){
            xpos += xdelta;
            ypos += ydelta;
            return true;
        } else {
            Serial.println("Encoder error, x-y-positions may be incorrect");
            return false;
        }
    }
}





#endif