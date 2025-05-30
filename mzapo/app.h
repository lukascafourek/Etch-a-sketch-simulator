/*
Variables and functions used by both menu and grafo screens
*/
#ifndef APP_H
#define APP_H

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
#define NOT_CLICKED 0
#define CLICKED 1
#define PLAY 100
#define STYLUS_COLOR 101
#define RESET_SCREEN 102
#define EXIT 103

// used font for text
#define font font_rom8x16

// hardware abstractions used
#include "hardware_abstraction/font_types.h"
#include "hardware_abstraction/mzapo_parlcd.h"
#include "hardware_abstraction/mzapo_phys.h"
#include "hardware_abstraction/mzapo_regs.h"

// map lcd address
extern void *parlcd_base;
// map both LEDs, LED line and knobs addresses
extern void *spiled_base;

// rgb color of LEDs
union rgb
{
  uint32_t d;
  struct
  {
    uint8_t b, g, r;
  };
};

// values of knobs
union knobs
{
  uint32_t d;
  struct
  {
    uint8_t b, g, r;
    bool bp : 1;
    bool gp : 1;
    bool rp : 1;
  };
};

// value of pixel on lcd display
union pixel
{
  uint16_t d;
  struct
  {
    int b : 5;
    int g : 6;
    int r : 5;
  };
};

extern int stylus_size;

extern char label_size[];

extern int target_coord[];

extern char label_coord[];

extern union rgb color;

extern union pixel chosen_color;

extern union pixel grafo_fb[DISPLAY_WIDTH][DISPLAY_HEIGHT];

// establishment of knob values
union knobs knobs(void);

// display given color on both LEDs
void set_leds(union rgb* color);

// display initialized frame buffer on lcd
void lcdframe(union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT]);

// initialize characters in rom8x16 font on lcd
void lcdchar(char c, int x, int y, union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT]);

// initialize strings to white text
void lcd_print_string(const char string[], int x, int y, int size, union pixel fb[DISPLAY_WIDTH][DISPLAY_HEIGHT]);

// initialize lcd to black screen
void set_screen(void);

// set bottom small frame as small block with white outline
void lcd_set_bottom_bar(void);

// display coords, stylus size and color on grafo screen
void print_bottom_info(union pixel* chosen_color);

#endif
