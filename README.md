# THIS IS CTU FEE WORK ARCHIVE

# Etch A Sketch simulator - user manual

### The game

<p> Etch A Sketch is a sandbox drawing game where the user has two knobs to move with a stylus on the display</p>

## Board peripherals required to play

![The board](/ico/board.png)

### Knobs

* Red, green and blue knob are located below the LCD display

#### The user can control the game with these knobs

**While in main menu:**<br> </p>

* <p>Turning any knob changes option selection. Clicking blue knob confirms the selection.<br></p>

**When in main menu (set stylus color option was selected):**<br> </p>

* <p>Turning knobs adds value of the knobs color.<br>
  e.g. turning red knob to the right adds red color value (0-255) turning left lowers said value.</p>
* <p>Clicking the blue knob will confirm color selection.</p>

**While in game screen:**<br> </p>

* <p>Turning red knob moves the stylus to the sides.<br>
  (left turn -> stylus goes to the left / right turn -> stylus goes to the right)<br></p>
* <p>Turning green knob moves the stylus up and down.<br>
  (left turn -> stylus goes up / right turn -> stylus goes down)<br></p>
* <p>Turning blue knob changes stylus size (from 0 up to 40 included). <br></p>
* <p>Clicking the blue knob will enter menu.</p>

### Two RGB LEDs

* First RGB LED is to the left of the knobs, second to the right.
* The RGB LEDs show selected color / color in use.

### LED line

* LED line is located below the knobs.
* Confirms knob clicks by a blink.
* Indicates whether the engine is in _select stylus color_ mode.

### Ethernet port

* Required to connect to a LAN network

### 5,5mm JACK Power supply

* Required to power up the board<br>

## Screens and their content

### Main menu

* PLAY - Opens game window
* SET STYLUS COLOR - Allows to change stylus color using knobs
* RESET SCREEN - Resets the screen of the game window
* EXIT - Closes the game

### Game screen

* Bottom infographic bar
  * Stylus center position on the left side (format xxx:yyy)
  * Stylus size (format xx)
  * Stylus color code (format 0xFFFF)
* Drawing canvas of size 480:300px

# Etch A Sketch simulator - technical report

## Code directory tree

### ico

* Contains images for ReadMe.md

### mzapo

* Contains code files and a code directory <br><br>

* **Makefile** - rules to build application and its remote execution
* **app.c** - implementation of functions used by both menu and grafo screens
* **app.h** - declarations of the variables and functions used by both menu and grafo screens
  * Functions working with peripherals through the abstracted hardware
* **grafo.c** - implementation of menu screen functions
* **grafo.h** - declaration of grafo screen functions
  * Code maintaining the game screen
* **main.c** - main file where the program runs
  * The file that is executed after compilation
* **menu.c** - implementations of menu functions
* **menu.h** - declaration of menu screen functions
  * Functions working with game main menu and starting / resetting game screen, point of exit
* **run_grafo.c** - implementation of run_grafo function
* **run_grafo.h** - declaration to use grafo functions in a run_grafo
  * Code executing grafo.h functions in correct order
* **run_menu.c** - implementation of menu screen functions
* **run_menu.h** - declaration of menu screen functions
  * Code executing menu.h functions in correct order
* **stylus_color_menu.c** - implementations of stylus_color_menu functions
* **stylus_color_menu.h** - declarations for stylus color selection
  * Menu allowing the selection and confirmation of the stylus color

#### hardware_abstraction

* Contains code that abstract hardware addresses <br><br>

* **font_rom8x16.c** - fixed width font
* **font_types.h** - font description structure type definition
* **mzapo_parlcd.h** - low level LCD display control and data write functions declaration
* **mzapo_parlcd.c** - low level LCD display control and data write functions implementation
* **mzapo_phys.h** - prototype declaration of the function which allows map physical address range into virtual/user/application address space
* **mzapo_phys.c** - implementation of the function which allows map physical address range into virtual/user/application address space
* **mzapo_regs.h** - definition of base addresses and registers fields of the peripherals provided by education design for MZ_APO kits

## Code compilation and execution

### SSH-agent

1. To connect to mzapo board, it is required to have root-key in directory ~/.ssh/mzapo-root-key

  > scp <cvut_login@postel.felk.cvut.cz>:/opt/zynq/ssh-connect/mzapo-root-key ~/.ssh/

* Replace 'cvut_login' with your own login

2. Set the root-key file permission to be readable, writable and executable

 > chmod go-rwx mzapo-root-key

3. Start the ssh-agent

  > eval $(ssh-agent -s)

4. Add the root key to the agent

  > ssh-add ~/.ssh/mzapo-root-key

### Compilation and execution

1. Set TARGET_IP in MakeFile to current board IP address

* In MakeFile:
>
>19. ...
>20. TARGET_IP ?= CURRENT_BOARD_IP
>21. ...
>
2. Run following command in the terminal

  > make run

* Instead of changing board IP in MakeFile, you can use current board IP as a parameter when executing

> make TARGET_IP=CURRENT_BOARD_IP run
>
## Dependencies for Linux OS

For Debian/Ubuntu

* Update your system

> sudo apt update <br>
> sudo apt upgrade <br>

### crossbuild-essential-armhf

* To be able to cross-compile the code

> sudo apt install crossbuild-essential-armhf <br>

### ddd - debug only
>
> sudo apt install ddd

### gdb-multiarch - debug only
>
> sudo apt install gdb-multiarch
