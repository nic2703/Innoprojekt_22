#include <Arduino.h>
#include <Servo.h>

#define MAX_X 1000 //TODO: choose a value here, this is the short side
#define MAX_Y 10000 //TODO: Choose a value here, this is the long side

#define MAX_SPEED_X 0.18 //TODO: Get a max speed calculation
#define MAX_SPEED_Y 0.32 //TODO: Get the max speed calculation

#define CORRECTION 1 //TODO: get Correction

#define cu(x) sq(x)*x
#define speed_to_bits(s) (sq(s)) //TODO: yoink from large drivers

#define SPEED_A 9
#define SPEED_B 8
#define DIR_A 3
#define DIR_B 11
#define BRAKE_A 12
#define BRAKE_B 13

#define LIMIT 2

struct Plotter{
    int x;
    int y;
    int z; //{0,1} for {down, up}
};

/**
 * @brief immediately stops the plotter, aborts the program
 * 
 */
void stop();

/**
 * @brief used for calibration, runs the plotter into the walls to help measure the positions
 * 
 * @param pins_x 
 * @param pins_y 
 * @return true 
 * @return false 
 */
bool run_into_walls(int[3], int[3]);

/**
 * @brief calibrates the plotter by running it into the limit switches and then measuring the time taken
 * 
 */
void calibrate();

/**
 * @brief Set the speed of of motors
 * 
 * @param pins array of the pins {speed, direction, brake}
 * @param speed speed to set the motors to, sets brakes, if speed == 0
 */
void set_speed(int[], int);

/**
 * @brief checks if the new position of the plotter would be out of bounds
 * 
 * @param dx 
 * @param dy 
 * @return true if out of bounds
 * @return false if not
 */
bool out_of_bounds(int, int);


/**
 * @brief draws a line in the specified direction
 * 
 * @param dx x distance to draw
 * @param dy y distance to draw
 */
void draw_line(int, int);