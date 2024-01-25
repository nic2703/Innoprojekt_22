#include <SoftwareSerial.h> // not in current path but included in the arduino environment
SoftwareSerial BTserial(2, 3); // RX | TX

#include<Wire.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTserial.begin(9600);
}

bool start = false;

void loop() {

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  /*convert to strings fro better communication*/
  String Ax = String(AcX / 2048.0);
  String Ay = String(AcY / 2048.0);
  String Az = String(AcZ / 2048.0);

  if (Serial.available() || start) {
    start = true;

    /*output data to the serial monitor*/
    Serial.print(Ax); Serial.print(", ");
    Serial.print(Ay); Serial.print(", ");
    Serial.println(Az); Serial.print(" ");

    if (BTserial.available())
    {
      /*send python compatible data*/
      BTserial.print(Ax); BTserial.print(", ");
      BTserial.print(Ay); BTserial.print(", ");
      BTserial.print(Az);
    }

  }


  delay(100);
}