/*
Implementation of run_menu function
*/
#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "run_menu.h"

int run_menu(bool type, union pixel* chosen_color)
{
  fprintf(stdout, "\nMenu screen\n");
  // if used after program launch
  if (!type)
  {
    initialize_menu(&menu);
    assert(&menu != NULL);
    change_stylus_color(false, &color, chosen_color);
    reset_or_initialize_grafo_screen(chosen_color);
  }
  display_menu(menu.play);
  // check values of knobs
  union knobs k = knobs();
  union knobs last_k = k;
  // prepare for menu position and 24th bit of blue knob if clicked
  int counter = 0, menu_exit;
  uint32_t mask = 0x07000000, result;
  // cycle to do actions according to menu position
  while (true)
  {
    // initialize last_k only if some knob has been turned a full one tooth of its mechanical wheel
    if ((k.r % 4) == 0 && (k.g % 4) == 0 && (k.b % 4) == 0)
    {
      last_k.r = k.r;
      last_k.g = k.g;
      last_k.b = k.b;
    }
    // update the values of knobs
    k = knobs();
    // if blue knob clicked, do appropriate action according to menu position
    if ((result = (k.d & mask) >> 24) & 1)
    {
      menu_exit = knob_clicked(counter);
      action_after_click(menu_exit, chosen_color);
      if (menu_exit == PLAY || menu_exit == EXIT || menu_exit == EXIT_FAILURE)
      {
        break;
      }
    }
    // use below function only if updated knob values differ and knob has been rotated a full one tooth
    if ((last_k.r != k.r && (k.r % 4) == 0) || (last_k.g != k.g && (k.g % 4) == 0) || (last_k.b != k.b && (k.b % 4) == 0))
    {
      menu_knob_rotation(&counter, menu, k, last_k);
    }
  }
  return menu_exit;
}
