/*
Header file to use menu and stylus_color_menu functions in a run_menu function as a "small main"
*/
#ifndef RUN_MENU_H
#define RUN_MENU_H

#include "menu.h"
#include "stylus_color_menu.h"

// function to use menu and stylus_color_menu functions
int run_menu(bool type, union pixel* chosen_color);

#endif
