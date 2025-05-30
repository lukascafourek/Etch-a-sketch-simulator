/*
Header file for grafo screen functions
*/
#ifndef GRAFO_H
#define GRAFO_H

#include "app.h"

// set stylus coordinates with knob values
void set_stylus_pos(union knobs k, int target_coord[], int last_x, int last_y);

// set coordinates to label_coord to use it further
void set_coord_text(int target_coord[]);

// change pixel color on the stylus coordinates to given color
void apply_stylus_color(int target_coord[], int stylus_size, union pixel* chosen_color);

// set stylus size according to blue knob rotation
int set_stylus_size(int size, union knobs k, int last_size);

// set stylus size to label_size to use it further
void set_size_text(int size);

#endif
