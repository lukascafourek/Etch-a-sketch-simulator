/*
Implementations of grafo functions
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

#include "grafo.h"

void set_stylus_pos(union knobs k, int target_coord[], int last_x, int last_y)
{
  // smooths the move
  int sw = 0;
  // RED knob rotated to the left
  if (last_x + sw < k.r)
  {
    target_coord[0]--;
  }
  // RED knob rotated to the right
  else if (last_x - sw > k.r)
  {
    target_coord[0]++;
  }
  // GREEN knob rotated to the left
  if (last_y + sw < k.g)
  {
    target_coord[1]--;
  }
  // GREEN knob rotated to the right
  else if (last_y - sw > k.g)
  {
    target_coord[1]++;
  }
  // check if frame buffer overflow would happen
  if (target_coord[0] < 0)
  {
    target_coord[0] = DISPLAY_WIDTH - 1;
  }
  if (target_coord[1] < 0)
  {
    target_coord[1] = DISPLAY_HEIGHT - 21;
  }
  else if (target_coord[1] > DISPLAY_HEIGHT - 21)
  {
    target_coord[1] = 0;
  }
  target_coord[0] = target_coord[0] % DISPLAY_WIDTH;
  target_coord[1] = target_coord[1] % DISPLAY_HEIGHT;
}

void set_coord_text(int target_coord[])
{
  // coords of stylus as hundreds, tens and ones
  int x_char[] = {0, 0, 0};
  int y_char[] = {0, 0, 0};
  x_char[0] = target_coord[0] % 10;
  x_char[1] = (target_coord[0] % 100 - x_char[0]) / 10;
  x_char[2] = (target_coord[0] - x_char[1] - x_char[0]) / 100;
  y_char[0] = target_coord[1] % 10;
  y_char[1] = (target_coord[1] % 100 - y_char[0]) / 10;
  y_char[2] = (target_coord[1] - y_char[1] - y_char[0]) / 100;
  // change the label_coord to appropriate ASCII code
  for (size_t i = 0; i < 6; i++)
  {
    if (i < 3)
    {
      label_coord[i] = x_char[2 - i] + 48;
    }
    else
    {
      label_coord[i + 1] = y_char[5 - i] + 48;
    }
  }
}

void set_size_text(int size) {
  // stylus size array, first element is tens, second one is ones
  int size_char[] = {0, 0};
  size_char[0] = size / 10;
  size_char[1] = size % 10;
  // change the label_size to appropriate ASCII code
  for (size_t i = 0; i < 2; i++)
  {
    label_size[i + 13] = size_char[i] + 48;
  }
}

void apply_stylus_color(int target_coord[], int stylus_size, union pixel* chosen_color)
{
  // use the stylus as a circle type
  int radius = stylus_size / 2;
  int center_x = target_coord[0];
  int center_y = target_coord[1];
  // change pixels in the radius to given color
  for (int i = 0; i < stylus_size; i++)
  {
    for (int j = 0; j < stylus_size; j++)
    {
      int x1 = (((target_coord[0] + i) % DISPLAY_WIDTH) + DISPLAY_WIDTH) % DISPLAY_WIDTH;
      int x2 = (((target_coord[0] - i) % DISPLAY_WIDTH) + DISPLAY_WIDTH) % DISPLAY_WIDTH;
      int y1 = (((target_coord[1] + j) % (DISPLAY_HEIGHT - 20)) + (DISPLAY_HEIGHT - 20)) % (DISPLAY_HEIGHT - 20);
      int y2 = (((target_coord[1] - j) % (DISPLAY_HEIGHT - 20)) + (DISPLAY_HEIGHT - 20)) % (DISPLAY_HEIGHT - 20);
      // calculate the distance from the current pixel to the center of the circle
      int dx1 = x1 - center_x;
      int dx2 = x2 - center_x;
      int dy1 = y1 - center_y;
      int dy2 = y2 - center_y;
      int distance_squared1 = dx1 * dx1 + dy1 * dy1;
      int distance_squared2 = dx1 * dx1 + dy2 * dy2;
      int distance_squared3 = dx2 * dx2 + dy1 * dy1;
      int distance_squared4 = dx2 * dx2 + dy2 * dy2;
      // check if the current pixel is within the circle's radius
      if (distance_squared1 <= radius * radius)
      {
        grafo_fb[x1][y1].d = chosen_color->d;
      }
      if (distance_squared2 <= radius * radius)
      {
        grafo_fb[x1][y2].d = chosen_color->d;
      }
      if (distance_squared3 <= radius * radius)
      {
        grafo_fb[x2][y1].d = chosen_color->d;
      }
      if (distance_squared4 <= radius * radius)
      {
        grafo_fb[x2][y2].d = chosen_color->d;
      }
    }
  }
}

int set_stylus_size(int size, union knobs k, int last_size)
{
  // smooths the move
  int sw = 2;
  // knob rotated to the left
  if (last_size + sw < k.b)
  {
    size--;
  }
  else if (last_size + sw > 240 && k.b < 20)
  {
    size--;
  }
  // knob rotated to the right
  else if (last_size - sw > k.b)
  {
    size++;
  }
  else if (last_size + sw < 20 && k.b > 240)
  {
    size++;
  }
  // maximum and minimum stylus sizes
  if (size < 1)
  {
    size = 1;
  }
  if (size > 50)
  {
    size = 50;
  }
  return size;
}
