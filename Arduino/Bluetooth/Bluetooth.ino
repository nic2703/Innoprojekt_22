// Basic Bluetooth sketch HC-06_01
// Connect the Hc-06 module and communicate using the serial monitor
//
// The HC-06 defaults to AT mode when first powered on.
// The default baud rate is 9600
// The Hc-06 requires all AT commands to be in uppercase. NL+CR should not be added to the command string
//

#include <SoftwareSerial.h> // not in current path but included in the arduino environment
SoftwareSerial BTserial(2, 3); // RX | TX

 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Enter AT commands:");
 
    // HC-06 default serial speed is 9600
    BTserial.begin(9600);  
}
 
void loop()
{
 
    // Keep reading from HC-06 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        Serial.write(BTserial.read());
    }
 
    // Keep reading from Arduino Serial Monitor and send to HC-06
    if (Serial.available())
    {
        BTserial.write(Serial.read());
    }
 
}
