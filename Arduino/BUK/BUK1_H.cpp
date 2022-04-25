#include "BUK1_H.h"
#include "Arduino.h";

//Motor functions here
inline void set_speed(pin spdPin, bit bitspeed){
    analogWrite(spdPin, bitspeed);
}

inline void set_brakes(pin brkPin, int state){
    digitalWrite(brkPin, state);
}

inline void set_direction(pin dirPin, int state){
    digitalWrite(dirPin, state);
}

//Servo functions here
inline void servo_goto(bit angle){
    PenServo.write(angle);
    delay(15);
}

inline void servo_up(){
    servo_goto(_SERVO_TOP);
}

inline void servo_down(){
    servo_goto(_SERVO_BOTTOM);
}

inline bit servo_angle(){
    return PenServo.read();
}

//Math functions here
inline bool outofbounds(BUKvec coords){
    if (coords[0]<= 0.0f || coords[0]>= _PAPER_WIDTH || coords[1]<= 0.0f || coords[1]>= _PAPER_LENGTH){
        Serial.print("DRAW || ERROR A2: Coordinates provided ");
        Serial.print(coords[0]);
        Serial.print(" , ");
        Serial.print(coords[1]);
        Serial.print(" are out of bounds.");
        return true;
    }
    return false;
}

BUKvec& BUKBezier(BUKvec& position, BUKvec& control1, BUKvec& control2, BUKvec& coords, unsigned int prec, unsigned int i){
    float t = i/prec;
    float x = cube(1-t)*position[0] + sq(1-t)*3*t*control1[0] + (1-t)*3*sq(t)*control2[0] + cube(t)*coords[0];
    float y = cube(1-t)*position[1] + sq(1-t)*3*t*control1[1] + (1-t)*3*sq(t)*control2[1] + cube(t)*coords[1];
    BUKvec subpoint(x, y);
    return subpoint;
}

BUKvec& BUKBezier(BUKvec& position, BUKvec& control1, BUKvec& coords, unsigned int prec, unsigned int i){
    float t = i/prec;
    float x = sq(1-t)*position[0] + (1-t)*2*t*control1[0] + sq(t)*coords[0];
    float y = sq(1-t)*position[1] + (1-t)*2*t*control1[1] + sq(t)*coords[1];
    BUKvec subpoint(x, y);
    return subpoint;
}

//Class functions here
BUKPlt::BUKPlt(){
    position = BUKvec(-1.0f, -1.0f);

    TIME_MAX = 10000000;
    _BRAKE_A = 9;
    _BRAKE_B = 8;
    _SPEED_A = 3;
    _SPEED_B = 11;
    _DIR_A = 12;
    _DIR_B = 13;
    _SERVO_LATCH = 4;
    _BUTTON_XTOP = 0;
    _BUTTON_XBTM = 0;
    _BUTTON_YTOP = 0;
    _BUTTON_YBTM = 0;
    _BUTTON_EMERGENCY = 0;

    xforward = HIGH;
    xback = LOW;
    yforward = HIGH;
    yback = LOW;
    Serial.println("Initializing Plotter");
    pinMode(_SPEED_A, OUTPUT);
    pinMode(_DIR_A, OUTPUT);
    pinMode(_BRAKE_A, OUTPUT);
    pinMode(_SPEED_B, OUTPUT);
    pinMode(_DIR_B, OUTPUT);
    pinMode(_BRAKE_B, OUTPUT);

    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    
    PenServo.attach(_SERVO_LATCH);

    pinMode(_BUTTON_XTOP, INPUT);
    pinMode(_BUTTON_XBTM, INPUT);
    pinMode(_BUTTON_YTOP, INPUT);
    pinMode(_BUTTON_YBTM, INPUT);
    pinMode(_BUTTON_EMERGENCY, INPUT);
}

BUKPlt::~BUKPlt(){
    Serial.println("Plotter object destroyed");
}


void BUKPlt::flipmotors(){
    pin brktemp = _BRAKE_A;
    pin spdtemp = _SPEED_A;
    pin dirtemp = _DIR_A;
    _BRAKE_A = _BRAKE_B;
    _SPEED_A = _SPEED_B;
    _DIR_A = _DIR_B;
    _BRAKE_B = brktemp;
    _SPEED_B = spdtemp;
    _DIR_B = dirtemp;
}

void BUKPlt::flipdirection(int forward, int backward){
    if (forward == HIGH){
        forward = LOW;
        backward = HIGH;
    } else {
        forward = HIGH;
        backward = LOW;
    }
}


void BUKPlt::outputBUKvec(BUKvec& coords){
    Serial.print(coords[0]);
    Serial.print(" , ");
    Serial.print(coords[1]);
}


void BUKPlt::servosetup(){
    Serial.println("SETUP || Please make sure the servo horn is detached!");
    delay(1000);
    servo_goto(0);
    for (int i = 0; i < 19; i++){
        //welcome to the waiting queue
        servo_goto(i*10);
        Serial.print("SETUP || Current angle: ");
        Serial.println(i*10);
        delay(500);
    }
    Serial.println("SETUP || Please modify _SERVO_TOP and _SERVO_BOTTOM in BUK1_H.h accordingly");
}

bool BUKPlt::calibrate(bit bitspeed){
    Serial.println("SETUP || General Calibration commenced");
    servo_up();
    Serial.print("SETUP || Do motors need to be switched?");
    int instance = this->iscorrectmotor(bitspeed);
    switch(instance){
        case -1: {Serial.println("SETUP || ERROR A1: Time limit passed"); return false;}
        case 0: {
            Serial.println("SETUP || Motors need to be switched");
            flipmotors();
            break;
        }
        case 1: {Serial.println("SETUP || Motors do not need to be flipped"); break;}
        default: {Serial.println("SETUP || ERROR C1"); return false;}
    }

    Serial.print("SETUP || Motor A: Does direction need to be changed?");
    instance = this->iscorrectdirectionA(bitspeed);
    switch(instance){
        case -1: {Serial.println("SETUP || ERROR A1: Time limit passed"); return false;}
        case 0: {
            Serial.println("SETUP || Motor A direction needs to be flipped");
            flipdirection(xforward, xback);
            break;
        }
        case 1: {Serial.println("SETUP || Motor A direction do not need to be flipped"); break;}
        default: {Serial.println("SETUP || ERROR C2"); return false;}
    }

    Serial.print("SETUP || Motor B: Does direction need to be changed?");
    instance = this->iscorrectdirectionB(bitspeed);
    switch(instance){
        case -1: {Serial.println("SETUP || ERROR A1: Time limit passed"); return false;}
        case 0: {
            Serial.println("SETUP || Motor B direction needs to be flipped");
            flipdirection(yforward, yback);
            break;
        }
        case 1: {Serial.println("SETUP || Motor B direction do not need to be flipped"); break;}
        default: {Serial.println("SETUP || ERROR C3"); return false;}
    }

    Serial.print("SETUP || Motor Setup Complete, moving to corner");
    instance = this->calibratecorner(bitspeed);
    switch(instance){
        case -2: {Serial.println("SETUP || ERROR A1: Time limit passed on y direction (Motor B)"); return false;}
        case -1: {Serial.println("SETUP || ERROR A1: Time limit passed on x direction (Motor A)"); return false;}
        case 1: {
            Serial.println("SETUP || Corner successfully reached. Preparing coordinates"); 
            servo_down();
            delay(500);
            servo_up();
            position = BUKvec(0.1f, 0.1f);
            Serial.print("SETUP || Position set to ");
            outputBUKvec(position);
            Serial.println('\0');
            return true;
        }
        default: {Serial.println("SETUP || ERROR C4"); return false;}
    }

}

int BUKPlt::iscorrectmotor(bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);

    set_direction(_DIR_A, xback);
    set_speed(_SPEED_A, bitspeed);
    set_brakes(_BRAKE_A, LOW);
    unsigned long int sandclock = micros();
    while (digitalRead(_BUTTON_XBTM) == LOW && digitalRead(_BUTTON_XTOP) == LOW 
    && digitalRead(_BUTTON_YBTM) == LOW && digitalRead(_BUTTON_YTOP) == LOW 
    && micros()<=sandclock+TIME_MAX){
        //welcome to the waiting queue
    }
    set_brakes(_BRAKE_A, HIGH);

    int instance = -2;
    if (micros()>=sandclock+TIME_MAX){
        instance = -1;
    }
    if (digitalRead(_BUTTON_XTOP) == HIGH){
        instance = 1;
    }
    if (digitalRead(_BUTTON_YBTM) == HIGH){
        instance = 0;
    }
    if (digitalRead(_BUTTON_XBTM) == HIGH){
        instance = 1;
    }
    if (digitalRead(_BUTTON_YTOP) == HIGH){
        instance = 0;
    }
    set_direction(_DIR_A, xforward);
    set_brakes(_BRAKE_A, LOW);
    delay(500);
    set_brakes(_BRAKE_A, HIGH);
    return instance;
}

int BUKPlt::iscorrectdirectionA(bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);

    set_direction(_DIR_A, xback);
    set_speed(_SPEED_A, bitspeed);
    set_brakes(_BRAKE_A, LOW);
    unsigned long int sandclock = micros();
    while (digitalRead(_BUTTON_XBTM) == LOW && digitalRead(_BUTTON_XTOP) == LOW && micros()<=sandclock+TIME_MAX){
        //welcome to the waiting queue
    }
    set_brakes(_BRAKE_A, HIGH);
    
    int instance = -2;
    if (micros()>=sandclock+TIME_MAX){
        instance = -1;
    }
    if (digitalRead(_BUTTON_XTOP) == HIGH){
        instance = 0;
    }
    if (digitalRead(_BUTTON_XBTM) == HIGH){
        instance = 1;
    }
    set_direction(_DIR_A, xforward);
    set_brakes(_BRAKE_A, LOW);
    delay(500);
    set_brakes(_BRAKE_A, HIGH);
    return instance;
}

int BUKPlt::iscorrectdirectionB(bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);

    set_direction(_DIR_B, yback);
    set_speed(_SPEED_B, bitspeed);
    set_brakes(_BRAKE_B, LOW);
    unsigned long int sandclock = micros();
    while (digitalRead(_BUTTON_YBTM) == LOW && digitalRead(_BUTTON_YTOP) == LOW && micros()<=sandclock+TIME_MAX){
        //welcome to the waiting queue
    }
    set_brakes(_BRAKE_B, HIGH);
    
    int instance = -2;
    if (micros()>=sandclock+TIME_MAX){
        instance = -1;
    }
    if (digitalRead(_BUTTON_YTOP) == HIGH){
        instance = 0;
    }
    if (digitalRead(_BUTTON_YBTM) == HIGH){
        instance = 1;
    }
    set_direction(_DIR_B, yforward);
    set_brakes(_BRAKE_B, LOW);
    delay(500);
    set_brakes(_BRAKE_B, HIGH);
    return instance;
}

int BUKPlt::calibratecorner(bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);

    set_direction(_DIR_A, xback);
    set_speed(_SPEED_A, bitspeed);
    set_brakes(_BRAKE_A, LOW);
    unsigned long int sandclock = micros();
    while (digitalRead(_BUTTON_XBTM) == LOW && micros()<=sandclock+TIME_MAX){
        //welcome to the waiting queue
    }
    set_brakes(_BRAKE_A, HIGH);

    if (micros()>=sandclock+TIME_MAX){
        return -1;
    }
    if (digitalRead(_BUTTON_XBTM) == HIGH){
        set_direction(_DIR_B, yback);
        set_speed(_SPEED_B, bitspeed);
        set_brakes(_BRAKE_B, LOW);
        sandclock = micros();
        while (digitalRead(_BUTTON_YBTM) == LOW && micros()<=sandclock+TIME_MAX){
        //welcome to the waiting queue
        }
        set_brakes(_BRAKE_B, HIGH);

        if (micros()>=sandclock+TIME_MAX){
            return -2;
        }
        if (digitalRead(_BUTTON_YBTM) == HIGH){
            set_direction(_DIR_A, xforward);
            set_direction(_DIR_B, yforward);
            set_brakes(_BRAKE_A, LOW);
            set_brakes(_BRAKE_B, LOW);
            delay(500);
            set_brakes(_BRAKE_A, HIGH);
            set_brakes(_BRAKE_B, HIGH);
            return 1;
        }
        return -3;
    }
    return 0;
}


bool BUKPlt::emergencystop(){
    if (digitalRead(_BUTTON_XBTM) == HIGH || digitalRead(_BUTTON_XTOP) == HIGH 
    || digitalRead(_BUTTON_YBTM) == HIGH || digitalRead(_BUTTON_YTOP) == HIGH
    || digitalRead(_BUTTON_EMERGENCY) == HIGH){
        //digitalWrite(12, HIGH); straight to interrupt
        Serial.println("EMERGENCY || ERROR A3: Emergency Stop initiated");
        return true;
    }
    return false;
}


bool BUKPlt::penM(BUKvec& coords, bit bitspeed){
    servo_up();
    Serial.println("penM || Moving to coordinates");
    if(!penL(coords, bitspeed)){
        return false;
    }
    servo_down();
    return true;
}

bool BUKPlt::penZ(BUKvec& coords, bit bitspeed){
    Serial.println("penZ || Moving to coordinates");
    if(!penL(coords, bitspeed)){
        return false;
    }
    servo_up();
    return true;
}

bool BUKPlt::penL(BUKvec& coords, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    if (outofbounds(coords)){
        return false;
    }
    BUKvec direction = coords-position;
    unsigned long int speedmmpers_X = BITS_TO_SPEED((float)bitspeed*(direction[0])/direction.norm());
    unsigned long int timeseconds_X = direction[0]/speedmmpers_X;
    unsigned long int timemicros_X = timeseconds_X*1000*1000;
    unsigned long int arrivaldate = micros()+timemicros_X;

    Serial.print("penL || Drawing to coordinates ");
    outputBUKvec(coords);
    Serial.print(" via direction ");
    outputBUKvec(direction);

    direction[0]>=0.0f?set_direction(_DIR_A, xforward):set_direction(_DIR_A, xback);
    direction[1]>=0.0f?set_direction(_DIR_B, yforward):set_direction(_DIR_B, yback);
    set_speed(_SPEED_A, (float)bitspeed*(direction[0])/direction.norm());
    set_speed(_SPEED_B, (float)bitspeed*(direction[1])/direction.norm());
    set_brakes(_BRAKE_A, LOW);
    set_brakes(_BRAKE_B, LOW);

    while(micros()<=arrivaldate){
        //welcome to the waiting queue
        if (emergencystop()){
            Serial.println(" E-STOP");
            set_brakes(_BRAKE_A, HIGH);
            set_brakes(_BRAKE_B, HIGH);
            return false;
        }
    }
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    Serial.println(" SUCCESS!");
    position = coords;
    return true;
}

bool BUKPlt::penH(float& h, bit bitspeed){
    BUKvec coords = BUKvec(h, position[1]);
    servo_down();
    if(!penL(coords, bitspeed)){
        return false;
    }
    return true;
}

bool BUKPlt::penV(float& v, bit bitspeed){
    BUKvec coords = BUKvec(position[0], v);
    servo_down();
    if(!penL(coords, bitspeed)){
        return false;
    }
    return true;
}


bool BUKPlt::penC(BUKvec& control1, BUKvec& control2, BUKvec& coords, unsigned int prec, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    if (prec<=0 || prec>= 25){
        return false;
    }

    Serial.print("penC || Calculating ");
    Serial.print(prec+1);
    Serial.println(" subpoints");

    BUKvec *subpoint = new BUKvec[prec];
    for (unsigned int i = 0; i<=prec; i++){
        subpoint[i] = BUKvec(BUKBezier(position, control1, control2, coords, prec, i));
        Serial.print("penC || Subpoint ");
        Serial.print(i);
        Serial.print(": ");
        outputBUKvec(subpoint[i]);
        Serial.println('\0');

        if (outofbounds(subpoint[i])){
            return false;
        }
    }

    for (unsigned int i = 0; i<=prec; i++){
        if (!penL(subpoint[i], bitspeed)){
            return false;
        }
    }
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    return true;
}

bool BUKPlt::penQ(BUKvec& control1, BUKvec& coords, unsigned int prec, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    if (prec<=0 || prec>= 25){
        return false;
    }

    Serial.print("penQ || Calculating ");
    Serial.print(prec+1);
    Serial.println(" subpoints");

    BUKvec *subpoint = new BUKvec[prec];
    for (unsigned int i = 0; i<=prec; i++){
        subpoint[i] = BUKvec(BUKBezier(position, control1, coords, prec, i));
        Serial.print("penQ || Subpoint ");
        Serial.print(i);
        Serial.print(": ");
        outputBUKvec(subpoint[i]);
        Serial.println('\0');

        if (outofbounds(subpoint[i])){
            return false;
        }
    }

    for (unsigned int i = 0; i<=prec; i++){
        if (!penL(subpoint[i], bitspeed)){
            return false;
        }
    }
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    return true;
}


bool BUKPlt::adjustright(unsigned int distance, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    unsigned long int speedmmpers_X = BITS_TO_SPEED(bitspeed);
    unsigned long int timeseconds_X = distance/speedmmpers_X;
    unsigned long int timemicros_X = timeseconds_X*1000*1000;
    unsigned long int arrivaldate = micros()+timemicros_X;

    Serial.print("adjustright || Moving distance ");
    Serial.print(distance);

    set_direction(_DIR_A, xforward);
    set_speed(_SPEED_A, bitspeed);
    set_brakes(_BRAKE_A, LOW);
    while(micros()<=arrivaldate){
        //welcome to the waiting queue
        if (emergencystop()){
            Serial.println(" E-STOP");
            set_brakes(_BRAKE_A, HIGH);
            set_brakes(_BRAKE_B, HIGH);
            return false;
        }
    }
    set_brakes(_BRAKE_A, HIGH);
    Serial.println(" SUCCESS!");
    position = position+BUKvec(distance, 0);
    return true;
}

bool BUKPlt::adjustleft(unsigned int distance, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    unsigned long int speedmmpers_X = BITS_TO_SPEED(bitspeed);
    unsigned long int timeseconds_X = distance/speedmmpers_X;
    unsigned long int timemicros_X = timeseconds_X*1000*1000;
    unsigned long int arrivaldate = micros()+timemicros_X;

    Serial.print("adjustleft || Moving distance ");
    Serial.print(distance);

    set_direction(_DIR_A, xback);
    set_speed(_SPEED_A, bitspeed);
    set_brakes(_BRAKE_A, LOW);
    while(micros()<=arrivaldate){
        //welcome to the waiting queue
        if (emergencystop()){
            Serial.println(" E-STOP");
            set_brakes(_BRAKE_A, HIGH);
            set_brakes(_BRAKE_B, HIGH);
            return false;
        }
    }
    set_brakes(_BRAKE_A, HIGH);
    Serial.println(" SUCCESS!");
    position = position+BUKvec(-distance, 0);
    return true;
}

bool BUKPlt::adjustup(unsigned int distance, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    unsigned long int speedmmpers_Y = BITS_TO_SPEED(bitspeed);
    unsigned long int timeseconds_Y = distance/speedmmpers_Y;
    unsigned long int timemicros_Y = timeseconds_Y*1000*1000;
    unsigned long int arrivaldate = micros()+timemicros_Y;

    Serial.print("adjustup || Moving distance ");
    Serial.print(distance);

    set_direction(_DIR_B, yforward);
    set_speed(_SPEED_B, bitspeed);
    set_brakes(_BRAKE_B, LOW);
    while(micros()<=arrivaldate){
        //welcome to the waiting queue
        if (emergencystop()){
            Serial.println(" E-STOP");
            set_brakes(_BRAKE_A, HIGH);
            set_brakes(_BRAKE_B, HIGH);
            return false;
        }
    }
    set_brakes(_BRAKE_B, HIGH);
    Serial.println(" SUCCESS!");
    position = position+BUKvec(0, distance);
    return true;
}

bool BUKPlt::adjustdown(unsigned int distance, bit bitspeed){
    set_brakes(_BRAKE_A, HIGH);
    set_brakes(_BRAKE_B, HIGH);
    unsigned long int speedmmpers_Y = BITS_TO_SPEED(bitspeed);
    unsigned long int timeseconds_Y = distance/speedmmpers_Y;
    unsigned long int timemicros_Y = timeseconds_Y*1000*1000;
    unsigned long int arrivaldate = micros()+timemicros_Y;

    Serial.print("adjustdown || Moving distance ");
    Serial.print(distance);

    set_direction(_DIR_B, yback);
    set_speed(_SPEED_B, bitspeed);
    set_brakes(_BRAKE_B, LOW);
    while(micros()<=arrivaldate){
        //welcome to the waiting queue
        if (emergencystop()){
            Serial.println(" E-STOP");
            set_brakes(_BRAKE_A, HIGH);
            set_brakes(_BRAKE_B, HIGH);
            return false;
        }
    }
    set_brakes(_BRAKE_B, HIGH);
    Serial.println(" SUCCESS!");
    position = position+BUKvec(0, -distance);
    return true;
}