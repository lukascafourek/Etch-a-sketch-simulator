/*
Main file where the program runs
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
#include "run_grafo.h"

int main(int argc, char *argv[])
{
  fprintf(stdout, "\nLaunching application...\n");
  // launch initialization of hardware addresses
  parlcd_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  assert(parlcd_base != NULL);
  parlcd_hx8357_init(parlcd_base);
  spiled_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  assert(spiled_base != NULL);
  // display menu after launch
  int ret = EXIT_SUCCESS;
  ret = run_menu(false, &chosen_color);
  // cycle between two main screens
  while (ret != EXIT)
  {
    ret = run_grafo(&chosen_color);
    ret = run_menu(true, &chosen_color);
  }
  // set MicroZed lcd and LEDs to exit the app
  color.d = 0x0;
  set_screen();
  set_leds(&color);
  lcdframe(grafo_fb);
  fprintf(stdout, "\nExiting application...\n\n");
  return EXIT_SUCCESS;
}
