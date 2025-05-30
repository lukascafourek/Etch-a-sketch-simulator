/*
Implementations of app functions
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

#include "app.h"

void *parlcd_base;

void *spiled_base;

int stylus_size = 1;

char label_size[] = "Stylus size:   ";

int target_coord[] = {10, 10};

char label_coord[] = "010x010";

union rgb color;

union pixel chosen_color;

union pixel grafo_fb[DISPLAY_WIDTH][DISPLAY_HEIGHT];

union knobs knobs(void)
{
  uint32_t *knobs = spiled_base + SPILED_REG_KNOBS_8BIT_o;
  return (union knobs){.d = *knobs};
}

void set_leds(union rgb* color)
{
  uint32_t val = 0;
  val |= color->r;
  val <<= 8;
  val |= color->g;
  val <<= 8;
  val |= color->b;
  *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_RGB1_o) = val;
  *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_RGB2_o) = val;
}

void lcdframe(union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT])
{
  parlcd_write_cmd(parlcd_base, 0x2c);
  for (int j = 0; j < DISPLAY_HEIGHT ; j++)
  {
    for (int i = 0; i < DISPLAY_WIDTH ; i++)
    {
      parlcd_write_data(parlcd_base, fb[i][j].d);
    }
  }
}

void lcdchar(char c, int x, int y, union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT])
{
  const font_bits_t *cb = font.bits + (c * font.height);
  for (int j = 0; j < font.height; j++)
  {
    for (int i = 0; i < font.maxwidth; i++)
    {
      if (cb[j] & (0x1 << (16 - i)))
      {
        fb[x + i][y + j].d = 0xffff;
      }
    }
  }
}

void lcd_print_string(const char string[], int x, int y, int size, union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT])
{
  for (size_t i = 0; i < size; i++)
  {
    lcdchar(string[i], x + i*8, y, fb);
  }
}

void set_screen(void)
{
  for (int j = 0; j < DISPLAY_HEIGHT; j++)
  {
    for (int i = 0; i < DISPLAY_WIDTH; i++)
    {
      grafo_fb[i][j].d = 0x0;
    }
  }
}

void lcd_set_bottom_bar(void)
{
  for (int j = DISPLAY_HEIGHT - 20; j < DISPLAY_HEIGHT; j++)
  {
    for (int i = 0; i < DISPLAY_WIDTH; i++)
    {
      grafo_fb[i][j].d = 0x0;
    }
  }
  for (int i = 0; i < DISPLAY_WIDTH; i++)
  {
    grafo_fb[i][DISPLAY_HEIGHT - 1].d = 0xFFFF;
    grafo_fb[i][DISPLAY_HEIGHT - 20].d = 0xFFFF;
  }
  for (int i = 0; i < 20; i++)
  {
    grafo_fb[0][i + DISPLAY_HEIGHT - 20].d = 0xFFFF;
    grafo_fb[DISPLAY_WIDTH - 1][i + DISPLAY_HEIGHT - 20].d = 0xFFFF;
  }
}

void print_bottom_info(union pixel* chosen_color)
{
  char label_color[7] = "0x";
  char color_string[5];
  sprintf(color_string, "%X", chosen_color->d);
  strcat(label_color, color_string);
  lcd_print_string(label_coord, 4, DISPLAY_HEIGHT - 18, sizeof(label_coord), grafo_fb);
  lcd_print_string(label_size, 100, DISPLAY_HEIGHT - 18, sizeof(label_size), grafo_fb);
  lcd_print_string(label_color, DISPLAY_WIDTH - strlen(label_color) * 8 - 4, DISPLAY_HEIGHT - 18, sizeof(label_color), grafo_fb);
}
