How to use Arduino IDE on VSC:
- Delete the Microsoft Store Arduino IDE via Settings->Remove Apps
- Download the Arduino ZIP!!! file from the Arduino website or the `.exe` installer
- Unzip of course
- Add the Arduino extension to VSC, open an .ino file to set window into Arduino mode
- Go to Settings->"arduino.path" and write the path to the Arduino folder (where the zip was unzipped) if you used the installer, the path is `C:\\Program Files (x86)\\Arduino`
- RESTART VSC
- Connect Arduino, at bottom, select:
    - \<Select Programmer\> = AVRISP mkII
    - \<Select Board Type\> = Arduino Uno
    - \<Select Serial Port\> = wherever you attached the Arduino, typical format: `COM3` {number can vary}
    - Click the plug symbol left of the Port and select `9600` as that is the standard. This button will in the future also serve to open the serial port

Alternatively : [A comprehensive video](https://www.youtube.com/watch?v=VfLTZcKCGfk)
