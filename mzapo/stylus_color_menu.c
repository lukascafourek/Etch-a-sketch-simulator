/*
Implementations of stylus_color_menu functions
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

#include "stylus_color_menu.h"

void change_stylus_color(bool type, union rgb* color, union pixel* chosen_color)
{
  // if used from first menu after program launch
  if (!type)
  {
    color->r = 0x1f;
    color->g = 0x3f;
    color->b = 0x1f;
    set_leds(color);
  }
  // if used from menu during the grafo game
  else
  {
    // check values of knobs
    union knobs k = knobs();
    union knobs last_k = k;
    // prepare for 24th bit of blue knob if clicked
    uint32_t mask = 0x07000000, result;
    // cycle for color selection
    while (true)
    {
      // initialize last_k only if some knob has been turned a full one tooth of its mechanical wheel
      if ((k.r % 4) == 0 && (k.g % 4) == 0 && (k.b % 4) == 0)
      {
        last_k.r = k.r;
        last_k.g = k.g;
        last_k.b = k.b;
      }
      // update tthe knob values
      k = knobs();
      // if blue knob clicked, break
      if ((result = (k.d & mask) >> 24) & 1)
      {
        break;
      }
      // use below function only if updated knob values differ and knob has been rotated a full one tooth
      if ((last_k.r != k.r && (k.r % 4) == 0) || (last_k.g != k.g && (k.g % 4) == 0) || (last_k.b != k.b && (k.b % 4) == 0))
      {
        stylus_color_knob_rotation(color, k, last_k);
      }
    }
  }
  // change the color of stylus to chosen color from selection before
  chosen_color->r = color->r;
  chosen_color->g = color->g;
  chosen_color->b = color->b;
  lcd_set_bottom_bar();
  print_bottom_info(chosen_color);
  fprintf(stdout, "Chosen color: 0x%X\n", chosen_color->d);
}

void stylus_color_knob_rotation(union rgb* color, union knobs k, union knobs last_k)
{
  // knob rotated to the left
  // RED knob
  if (k.r > last_k.r || (k.r < 64 && last_k.r >= 192))
  {
    if (color->r == 0x0)
    {
      color->r = 0x1f;
    }
    else
    {
      color->r -= 1;
    }
  }
  // GREEN knob
  else if (k.g > last_k.g || (k.g < 64 && last_k.g >= 192))
  {
    if (color->g == 0x0)
    {
      color->g = 0x3f;
    }
    else
    {
      color->g -= 1;
    }
  }
  // BLUE knob
  else if (k.b > last_k.b || (k.b < 64 && last_k.b >= 192))
  {
    if (color->b == 0x0)
    {
      color->b = 0x1f;
    }
    else
    {
      color->b -= 1;
    }
  }
  // knob rotated to the right
  // RED knob
  else if (k.r < last_k.r || (k.r >= 192 && last_k.r < 64))
  {
    if (color->r == 0x1f)
    {
      color->r = 0x0;
    }
    else
    {
      color->r += 1;
    }
  }
  // GREEN knob
  else if (k.g < last_k.g || (k.g >= 192 && last_k.g < 64))
  {
    if (color->g == 0x3f)
    {
      color->g = 0x0;
    }
    else
    {
      color->g += 1;
    }
  }
  // BLUE knob
  else if (k.b < last_k.b || (k.b >= 192 && last_k.b < 64))
  {
    if (color->b == 0x1f)
    {
      color->b = 0x0;
    }
    else
    {
      color->b += 1;
    }
  }
  set_leds(color);
}
