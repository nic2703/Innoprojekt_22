#include "PlotterV2.h"

/**
 * @brief Draws a line from the current position using
 * @param dx delta x
 * @param dy delta y
 * @returns true | false if the task succeeded and false if it failed
 **/
bool draw_line(const Vec &);

//CIRCLES

/**
 * @brief Quadratic Bézier curve
 * 
 * @return true 
 * @return false 
 */
bool b_quad();

/**
 * @brief Cubic Bézier curve
 * 
 * @return true 
 * @return false 
 */
bool b_cubic();

/**
 * @brief wraps a graph around the origin in the style of a mandala
 * 
 * @return true 
 * @return false 
 */
bool draw_mandala();
