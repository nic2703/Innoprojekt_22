#include "PlotterV2.h"


Servo servo;
Plt p_plot;

/**
 * @brief Draws a line from the current position using
 * @param dx delta x
 * @param dy delta y
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_line(Vec &);

/**
 * @brief Draws a line to the nearest point on the circle radius, than starts drawing a circle segent up to the specified angle
 * @param midpoint circle midpoint coordinates
 * @param radius radius
 * @param arc arc length in radians within range [-2*pi, 2*pi]
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_circle_segment(Vec &, int, double);

/**
 * @brief Draws a line to a specified point on the circle radius, than starts drawing a circle segent up to the specified angle
 * @param midpoint circle midpoint coordinates
 * @param radius radius
 * @param arc arc length in radians within range [-2*pi, 2*pi]
 * @param start_angle angle relative to x-axis at which to start the circle segment
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_circle_segment(Vec &, int, double, double);

/**
 * @brief Quadratic Bézier curve
 * 
 * @return true 
 * @return false 
 */
bool b_quad();

/**
 * @brief wraps a graph around the origin in the style of a mandala
 * 
 * @return true 
 * @return false 
 */
bool draw_mandala();
