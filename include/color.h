#ifndef COLOR_H
#define COLOR_H


#include "vec3.h"

using color = vec3;

void write_color(std::ostream &out, const color &pixel_color);

double linear_to_gamma(double linear_component);
#endif