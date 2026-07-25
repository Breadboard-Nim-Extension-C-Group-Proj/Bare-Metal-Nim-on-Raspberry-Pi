// This file contains the main execution of the nim game 

#include <stdbool.h> 

#include "nim.h"

int main() {
    
    // These variables initialise the last time each button was pressed
    int select_pile_btn_time = 0; 
    int select_count_btn_time = 0; 
    int confirm_btn_time = 0; 

    // These variables initialise the last state of each button 
    bool select_pile_btn_state = false; 
    bool select_count_btn_state = false; 
    bool confirm_btn_state = false;  

    // This variable initialises the last time LEDs were blinked 
    uint32_t last_blink_time = 0;
    
    // Initialise the pins 
    init_pins(&hw_config); 

    // The game runs in an infinite loop, where each game is separated by a button press which restars the game 
    while (true) {
        
        // Every new game, we initialise the game state 
        init_game(&game_state);  

        // needs_render variable is a variable that makes sure LEDs blink only when some time has passed, mainly BLINK_INTERVAL
        bool needs_render = true;
        last_blink_time = get_system_time_ms();

        // This loop is responsible for a single game 
        while (!game_state.is_game_over) {
            
            // If the pile selector button is pressed, we change the pile to the next one in the list
            if (check_button_press(hw_config.button_select_pile, &select_pile_btn_time, &select_pile_btn_state)) {
                change_pile(&game_state); 
                needs_render = true;
            }
            
            // If the count selector button is pressed, we increase the amount of objects we are selecting 
            if (check_button_press(hw_config.button_select_count, &select_count_btn_time, &select_count_btn_state)) {
                change_num(&game_state); 
                needs_render = true;
            }

            // If the confirm button is pressed, the move is made 
            if (check_button_press(hw_config.button_confirm, &confirm_btn_time, &confirm_btn_state)) {
                apply_move(&game_state); 
                needs_render = true;
            }
            
            // if the BLINK_INTERVAL has passed, we flip the state of the selected LEDs, and set needs_render to true so that we can render the LEDs below 
            uint32_t current_time = get_system_time_ms();
            if (current_time - last_blink_time > BLINK_INTERVAL) {
                game_state.blinking_state = (game_state.blinking_state == ON) ? OFF : ON;
                last_blink_time = current_time;
                needs_render = true; 
            }

            // If the BLINK_INTERVAL has passed, we render the LEDs 
            if (needs_render) {
                render_leds(); 
                needs_render = false; 
            }
        } 

        // Since the confirm button is also used to begin new round, we record the last time it was pressed, as well as its last state 
        int end_game_time = get_system_time_ms();
        bool end_game_state = read_pin(hw_config.button_confirm);

        // The game has ended, so we display the LED output of the ended game 
        end_game(&game_state, &end_game_time, &end_game_state);
    }
    
    return 0;
}