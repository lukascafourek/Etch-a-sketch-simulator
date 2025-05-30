/*
Implementation of run_grafo function
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

#include "run_grafo.h"

int run_grafo(union pixel* chosen_color) 
{
  fprintf(stdout, "\nGrafo screen active\n");
  lcdframe(grafo_fb);
  // check status of each knob
  union knobs k = knobs();
  int last_x, last_y, last_size;
  // cycle to do actions on grafo screen
  while (true)
  {
    last_x = k.r;
    last_y = k.g;
    last_size = k.b;
    // update knob values
    k = knobs();
    // prepare for 24th bit of knob value if blue knob clicked
    uint32_t mask = 0x07000000, result;
    // if blue knob clicked, light LED line on and off and then break
    if ((result = (k.d & mask) >> 24) & 1)
    {
      // light the LED line on and then off
      fprintf(stdout, "Knob clicked, entering menu\n");
      *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0xFFFFFFFF;
      sleep(1);
      *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0x0;
      break;
    }
    // do actions with stylus coords, size and color
    set_stylus_pos(k, target_coord, last_x, last_y);
    stylus_size = set_stylus_size(stylus_size, k, last_size);
    apply_stylus_color(target_coord, stylus_size, chosen_color);
    // set coords and size labels to be displayed after
    set_coord_text(target_coord);
    set_size_text(stylus_size);
    // display frame buffer with stylus modifications
    lcd_set_bottom_bar();
    print_bottom_info(chosen_color);
    lcdframe(grafo_fb);
  }
  return EXIT;
}
