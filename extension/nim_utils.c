// This file defines the utility functions for the nim game

#include "nim.h"

// For the optimisation of equal play, each pile can be randomly initialised to 2, 4, or 5 LEDs with HIGH state
int random_piles[3] = {2, 4, 5};

// Global PinsState Structure
PinsState hw_config = {
    .b_leds = {24, 23, 18, 15, 14},
    .y_leds = {16, 12, 7, 8, 25},
    .g_leds = {4, 3, 2, 21, 20},
    .r_leds = {9, 10, 22, 27, 17},
    .button_select_pile = 6,
    .button_select_count = 5,
    .button_confirm = 11 
};

// Global GameState Structure 
GameState game_state;

void render_leds() {
    
    // For every pile... 
    for(int i = 0; i < PILE_COUNT; i++){ 
        int pile_count = get_pile_count(&game_state, i);    // Get the number of objects in each pile
        int *pin_state = get_pin_state(&hw_config, i);      // Get state of all pins in each pile 

        bool is_selected_pile = (i == game_state.move.pile);    // Flag which determines if the pile is the pile that is selected as the move (it should blink)
        int move_count = game_state.move.num_to_remove;         // Number of objects to remove from that pile (Only meaningful when is_selected_pile is true)

        // For every LED... 
        for(int j = 0; j < LEDS_COUNT; j++){

            if (j >= pile_count) {
                set_pin(pin_state[j], OFF);     // LEDs that are not representing objects anymore (because some were removed as game progresses) are turned OFF
            } else {
                bool is_blinking_target = is_selected_pile && (j < move_count);     // if is_selected_pile is true, and LED represents actual object in the pile:
                                                                                    // Then is_blinking_target is true, so these LEDs must blink 

                // This block enables LEDs to blink
                if (is_blinking_target && !game_state.blinking_state) {
                    set_pin(pin_state[j], OFF);     // If LEDs must be off, they're set to OFF 
                } else {
                    set_pin(pin_state[j], ON);      // Otherwise, both blinking LEDs and other active LEDs in the game are set to ON
                }
            }
        }
    }
}

// This function returns a random number for the pile 
int get_rand() {
    static int rand_calls = 0;  // This variable keeps track of calls to get_rand() function 
    rand_calls++; 
    return random_piles[(get_system_time_ms() + rand_calls) % 3];   // returns either 2, 4, or 5
}

// This function implements logic of what happens when the user presses a button 
bool check_button_press(int pin, int *last_debounce_time, bool *was_pressed) {

    // Get the current time, and the current state of the button 
    int current_time = get_system_time_ms();  
    bool current_state = read_pin(pin); 

    // This if-block is only executed when sufficient time passes after pressing the button (Read more about physical buttons debouncing)
    if (current_time - *last_debounce_time > DEBOUNCE_DELAY) {

        *last_debounce_time = current_time; // Reset last_debounce_time to current time
        if (current_state && !*was_pressed) {
            // Rising edge: Button was not pressed before, but is pressed npw 
            *was_pressed = true; 
            return true; 
        } else if (!current_state && *was_pressed) {
            // Falling edge: Button was pressed before but isn't pressed anymore 
            *was_pressed = false; 
        }
    }

    return false; 
}

// Sets LED pins so that they display letter A in morse code
void display_a() {
    set_pin(hw_config.r_leds[2], true); 

    set_pin(hw_config.y_leds[0], true); 
    set_pin(hw_config.y_leds[1], true); 
    set_pin(hw_config.y_leds[2], true); 
    set_pin(hw_config.y_leds[3], true); 
    set_pin(hw_config.y_leds[4], true); 
}

// Sets LED pins so that they display letter B in morse code
void display_b() {
    set_pin(hw_config.g_leds[0], true); 
    set_pin(hw_config.g_leds[1], true); 
    set_pin(hw_config.g_leds[2], true); 
    set_pin(hw_config.g_leds[3], true); 
    set_pin(hw_config.g_leds[4], true);

    set_pin(hw_config.b_leds[0], true); 
    set_pin(hw_config.b_leds[2], true);
    set_pin(hw_config.b_leds[4], true); 
}

// Resets each pile to OFF
void reset() {
    reset_pile(hw_config.r_leds);
    reset_pile(hw_config.g_leds); 
    reset_pile(hw_config.y_leds); 
    reset_pile(hw_config.b_leds); 
}

// Resets each LED in the pile to OFF
void reset_pile(int *pile) {
    for (int i = 0; i < LEDS_COUNT; i++) {
        set_pin(pile[i], false); 
    }
}

// Delays the execution by specified time
void delay_ms(uint32_t ms) {
    uint32_t start = get_system_time_ms();
    while (get_system_time_ms() - start < ms) {
    }
}