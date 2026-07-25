// This file contains definitions and function headers of the nim game 

#ifndef NIM_H
#define NIM_H

#include <stdint.h>
#include <stdbool.h>

#define LEDS_COUNT 5
#define PILE_COUNT 4

#define BLINK_INTERVAL 300

#define DEBOUNCE_DELAY 75 

// Game Logic Structures

typedef enum { PlayerA, PlayerB } CurrentPlayer;    // This enum defines the current player whose turn it is to move
typedef enum { R, G, Y, B } Pile;                   // This enum defines 4 piles that nim game has 
typedef enum { OFF, ON } LEDState;                  // This enum defines state of the LED, either ON or OFF

// This structure describes the current move to be processed 
typedef struct CurrentMove {
    Pile pile;                                      // The chosen pile
    int num_to_remove;                              // The number of objects to be removed from the chosen pile 
} CurrentMove;

// This structure describes the state of the game 
typedef struct GameState {
    CurrentPlayer player;                           // The current player to move
    CurrentMove move;                               // The current move to be made
    LEDState blinking_state;                        // The state of the blinking LEDs
    int g_pile;                                     // Number of objects left in the green pile
    int b_pile;                                     // Number of objects left in the blue pile
    int r_pile;                                     // Number of objects left in the red pile (changed to white LEDs)
    int y_pile;                                     // Number of objects left in the yellow pile
    bool is_game_over;                              // Flag that tells whether the game is over
} GameState;

// Hardware Manipulation Structures 

// Tjis structure describes the state of the pins used by the Raspberry PI 
typedef struct PinsState {
    int g_leds[LEDS_COUNT];                         // Pins that connect to green LEDs
    int b_leds[LEDS_COUNT];                         // Pins that connect to blue LEDs
    int r_leds[LEDS_COUNT];                         // Pins that connect to red LEDs (changed to white LEDs)
    int y_leds[LEDS_COUNT];                         // Pins that connect to yellow LEDs 
    int button_select_pile;                         // Pin that connects to the pile selector button (Green button)
    int button_select_count;                        // Pin that connects to the count select button (Red button)
    int button_confirm;                             // Pin that connects to the confirm button (Black button)
} PinsState;

// pins.c (Hardware Abstraction Layer)
void set_pin_mode(int pin, bool is_output);         // This function sets the mode for a pin
void set_pin(int pin, bool state);                  // This function sets the value for the output pins
bool read_pin(int pin);                             // This function reads the value from the input pin 
uint32_t get_system_time_ms(void);                  // This function gets the time in ms emerged from the beginning of the game
void init_pins(PinsState *state);                   // This function initialises all the pins used in the game
void init_pins_for_pile(int *pile);                 // This function initialises all the pins for a particular pile
void enable_pull_up(int pin);                       // This function enables the pull-up resistor for a certain pin 

// gamestate.c (Game Logic)
void init_game(GameState* state);                   // This function initialises the global GameState structure at the beginning of the game
void end_game(GameState* state, int *last_debounce_time, bool *was_pressed);    // This function prints out the output message for the game end 
void apply_move(GameState* state);                  // This function applies the chosen move to the state of the game (Executed when confirm button is pressed)
void change_pile(GameState* state);                 // This function changes the selected pile (Executed when select_pile button is pressed)
void change_num(GameState* state);                  // This function changes the number of objects selected in a pile (Executed when select_count is pressed)
int get_pile_count(GameState* state, Pile p);       // This function gets the number of objects left in a specified pile 
int *get_pin_state(PinsState* state, Pile p);       // This function gets the state of the pin 

// nim_utils.c
void render_leds();                                 // This function renders the LEDs during the game
int get_rand();                                     // This function gets a value that is either 2, 4, or 5 (Read more in nim.c)
bool check_button_press(int pin, int *last_debounce_time, bool *was_pressed);   // This button returns true if it detects that certain button was pressed
void display_a();                                   // Whenever PlayerA wins, this function displays letter A in morse code
void display_b();                                   // Whenever PlayerB wins, this function displays letter B in morse code
void reset();                                       // Together with display_a, and display_b, this function is used in endgame output. It sets LEDs to LOW
void reset_pile(int *pile);                         // This function resets a pile of LEDs to LOW 
 void delay_ms(uint32_t ms);                        // This function delays the whole execution of the program by specified amount of time 

extern PinsState hw_config;                         // Global PinsState structure
extern GameState game_state;                        // Global GameState structure
extern int random_piles[3];                         // This array contains possible initial configurations for each pile (2, 4, or 5) 

#endif