/*
Implementations of menu functions
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

#include "menu.h"
#include "stylus_color_menu.h"

struct menu menu;

void display_menu(union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT])
{
  lcd_print_string("PLAY", 240 - 2*8, 39, sizeof("PLAY"), fb);
  lcd_print_string("STYLUS COLOR", 240 - 6*8, 116, sizeof("STYLUS COLOR"), fb);
  lcd_print_string("RESET SCREEN", 240 - 6*8, 192, sizeof("RESET SCREEN"), fb);
  lcd_print_string("EXIT", 240 - 2*8, 268, sizeof("EXIT"), fb);
  lcdframe(fb);
}

void initialize_menu(struct menu* menu)
{
  for (int j = 0; j < DISPLAY_HEIGHT ; j++)
  {
    for (int i = 0; i < DISPLAY_WIDTH ; i++)
    {
      // PLAY button
      if ((j >= 14 && j < 78) && (i >= 120 && i < 360))
      {
        menu->play[i][j].r = 0x1f;
        menu->stylus_color[i][j].d = 0x0;
        menu->reset_screen[i][j].d = 0x0;
        menu->exit[i][j].d = 0x0;
      }
      // STYLUS COLOR button
      else if ((j >= 90 && j < 154) && (i >= 120 && i < 360))
      {
        menu->play[i][j].d = 0x0;
        menu->stylus_color[i][j].r = 0x1f;
        menu->reset_screen[i][j].d = 0x0;
        menu->exit[i][j].d = 0x0;
      }
      // RESET SCREEN button
      else if ((j >= 166 && j < 230) && (i >= 120 && i < 360))
      {
        menu->play[i][j].d = 0x0;
        menu->stylus_color[i][j].d = 0x0;
        menu->reset_screen[i][j].r = 0x1f;
        menu->exit[i][j].d = 0x0;
      }
      // EXITT button
      else if ((j >= 242 && j < 306) && (i >= 120 && i < 360))
      {
        menu->play[i][j].d = 0x0;
        menu->stylus_color[i][j].d = 0x0;
        menu->reset_screen[i][j].d = 0x0;
        menu->exit[i][j].r = 0x1f;
      }
      else
      {
        menu->play[i][j].d = 0x0;
        menu->stylus_color[i][j].d = 0x0;
        menu->reset_screen[i][j].d = 0x0;
        menu->exit[i][j].d = 0x0;
      }
    }
  }
}

int knob_clicked(int counter)
{
  int ret = EXIT_FAILURE;
  if (counter == 0)
  {
    fprintf(stdout, "Clicked Play button\n");
    ret = PLAY;
  }
  else if (counter == 1)
  {
    fprintf(stdout, "Clicked Stylus color button\n");
    ret = STYLUS_COLOR;
  }
  else if (counter == 2)
  {
    fprintf(stdout, "Clicked Reset screen button\n");
    ret = RESET_SCREEN;
  }
  else if (counter == 3)
  {
    fprintf(stdout, "Clicked Exit button\n");
    ret = EXIT;
  }
  else
  {
    fprintf(stderr, "ERROR: invalid value of counter!\n");
  }
  return ret;
}

void menu_knob_rotation(int* counter, struct menu menu, union knobs k, union knobs last_k)
{
  // knob rotated to the left
  if ((k.r > last_k.r || (k.r < 64 && last_k.r >= 192)) || 
  (k.g > last_k.g || (k.g < 64 && last_k.g >= 192)) || 
  (k.b > last_k.b || (k.b < 64 && last_k.b >= 192))) 
  {
    if (*counter > 0)
    {
      *counter -= 1;
    }
    if (*counter == 0)
    {
      display_menu(menu.play);
    }
    else if (*counter == 1)
    {
      display_menu(menu.stylus_color);
    }
    else if (*counter == 2)
    {
      display_menu(menu.reset_screen);
    }
    else
    {
      fprintf(stderr, "ERROR: invalid value of counter!\n");
    }
  }
  // knob rotated to the right
  else if ((k.r < last_k.r || (k.r >= 192 && last_k.r < 64)) || 
  (k.g < last_k.g || (k.g >= 192 && last_k.g < 64)) || 
  (k.b < last_k.b || (k.b >= 192 && last_k.b < 64))) 
  {
    if (*counter < 3)
    {
      *counter += 1;
    }
    if (*counter  == 1)
    {
      display_menu(menu.stylus_color);
    }
    else if (*counter  == 2)
    {
      display_menu(menu.reset_screen);
    }
    else if (*counter  == 3)
    {
      display_menu(menu.exit);
    }
    else
    {
      fprintf(stderr, "ERROR: invalid value of counter!");
    }
  }
}

void reset_or_initialize_grafo_screen(union pixel* chosen_color)
{
  set_screen();
  lcd_set_bottom_bar();
  print_bottom_info(chosen_color);
  fprintf(stdout, "Grafo screen set to initial value\n");
}

void action_after_click(int menu_exit, union pixel* chosen_color)
{
  if (menu_exit == PLAY || menu_exit == EXIT)
  {
    *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0xFFFFFFFF;
    sleep(1);
    *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0x0;
  }
  else if (menu_exit == STYLUS_COLOR)
  {
    *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0xFFFFFFFF;
    sleep(1);
    change_stylus_color(true, &color, chosen_color);
    sleep(1);
    *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0x0;
  }
  else if (menu_exit == RESET_SCREEN)
  {
    *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0xFFFFFFFF;
    reset_or_initialize_grafo_screen(chosen_color);
    sleep(1);
    *(volatile uint32_t*)(spiled_base + SPILED_REG_LED_LINE_o) = 0x0;
  }
  else
  {
    fprintf(stderr, "ERROR: invalid value of menu_exit!\n");
  }
}
