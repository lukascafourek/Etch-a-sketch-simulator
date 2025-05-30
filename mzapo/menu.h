/*
Header file for menu screen functions
*/
#ifndef MENU_H
#define MENU_H

#include "app.h"

// each possible menu position frame buffers
struct menu {
  union pixel play[DISPLAY_WIDTH][DISPLAY_HEIGHT];
  union pixel stylus_color[DISPLAY_WIDTH][DISPLAY_HEIGHT];
  union pixel reset_screen[DISPLAY_WIDTH][DISPLAY_HEIGHT];
  union pixel exit[DISPLAY_WIDTH][DISPLAY_HEIGHT];
};

extern struct menu menu;

// uses lcdframe and lcd_print_string functions to display menu
void display_menu(union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT]);

// initialize each menu position
void initialize_menu(struct menu* menu);

// determine what action will be performed according to menu position after knob click
int knob_clicked(int counter);

// display menu position with chosen button according to knob rotation
void menu_knob_rotation(int* counter, struct menu menu, union knobs k, union knobs last_k);

// set the grafo screen to initial value
void reset_or_initialize_grafo_screen(union pixel* chosen_color);

// light LED line on and off after knob click and 
// possibility to reset screen or choose stylus color according to menu position
void action_after_click(int menu_exit, union pixel* chosen_color);

#endif
