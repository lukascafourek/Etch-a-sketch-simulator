/*
Header file for stylus color selection
*/
#ifndef STYLUS_COLOR_MENU_H
#define STYLUS_COLOR_MENU_H

#include "app.h"

// change the stylus color to chosen values and display it on LEDs 
// and save it to pixel to use it further on lcd
void change_stylus_color(bool type, union rgb* color, union pixel* chosen_color);

// change the color commponent (R, G or B) according to knob rotation
void stylus_color_knob_rotation(union rgb* color, union knobs k, union knobs last_k);

#endif
