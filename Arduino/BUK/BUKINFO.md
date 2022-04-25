# Inno22BUK
Bukh version of the Innoprojekt22 Code.
Version 0.1 released for Inno22

BUK1_H_ is the first iteration. It contains all the following:
- Plotter initialization, including all the motor and servo objects
- Initialization and calibration of coordinate system, include "test" csv (more below)
- Can read input from a CSV file (modified SVG code)
- Can draw straight line input curves and arcs

Perhaps to be added:
- Dynamic precision on high curvature curve segments

ERROR HANDLING
Error A1: The TIME_MAX maximum time for the execution of a motor command has been exceeded. The motors have been turned off.

Error A2: The coordinates provided to one of the following functions
- penM()
terminates outside of the allowed drawing area (0 < x < _PAPER_WIDTH && 0 < y < _PAPER_LENGTH)

Error A3: An emergency stop was initiated. One of the edge sensors or the emergency stop button was pressed.

Error C1: In the calibrate function, if no satisfactory integer is returned by the iscorrectmotor function (not -1, 0 or 1), this means that no button has been hit and the time has NOT been exceeded, which means the function has terminated for some other issue.

Error C2: Analog to C1 but for the iscorrectdirectionA function

Error C3: Analog to C2 but for the iscorrectdirectionB function

Error C4: This occurs when no satisfactory return is given for the calibratecorner function. This means that not all buttons were reached but the time limit has not elapsed, or any other default case was returned.

NEEDED
- Small code snippet to output servo angles when starting the build

POTENTIAL ISSUES
- Servo functions might need to be under the BUKPlt constructor to have the Servo object initialized´
- Do non-member servo functions (aka servo_up(), servo_down(), etc...) have access to the Servo object when it it initialized in the BUKPlt constructor?