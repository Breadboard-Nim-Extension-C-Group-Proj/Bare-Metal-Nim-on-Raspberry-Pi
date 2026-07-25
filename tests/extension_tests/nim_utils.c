#include "nim.h"

// For the optimisation of equal play, each pile can be randomly initialised to 2, 4, or 5 LEDs with HIGH state
int random_piles[3] = {2, 4, 5};

PinsState hw_config = {
    .b_leds = {14, 15, 18, 23, 24},
    .y_leds = {25, 8, 7, 12, 16},
    .g_leds = {20, 21, 2, 3, 4},
    .r_leds = {17, 27, 22, 10, 9},
    .button_select_pile = 6,
    .button_select_count = 5,
    .button_confirm = 11 
};

GameState game_state;



void render_leds() {
    for(int i = 0; i < 4; i++){ 
        int pile_count = get_pile_count(&game_state, i);
        int *pin_state = get_pin_state(&hw_config, i);

        bool is_selected_pile = (i == game_state.move.pile);
        int move_count = game_state.move.num_to_remove;

        for(int j = 0; j < LEDS_COUNT; j++){
            if (j >= pile_count) {
                // set_pin(pin_state[j], OFF);
            } else {
                bool is_blinking_target = is_selected_pile && (j < move_count);

                if (is_blinking_target && !game_state.blinking_state) {
			//set_pin(pin_state[j], OFF); 
                } else {
                    	//set_pin(pin_state[j], ON);  
                }
            }
        }
    }
}

// This function returns a random number for the pile 
int get_rand() {
    return 4;//random_piles[get_system_time_ms() % 3]; 
}

// This function implements logic of what happens when the user presses a button 
bool check_button_press(int pin, int *last_debounce_time, bool *was_pressed) {

    int current_time = 0;//get_system_time_ms(); 
    bool current_state = true;//read_pin(pin); 

    if (current_time - *last_debounce_time > DEBOUNCE_DELAY) {
        if (current_state && !*was_pressed) {
            *last_debounce_time = current_time; 
            *was_pressed = true; 
            return true; 
        }

        if (!current_state) {
            *was_pressed = false; 
        }
    }

    return false; 
}

void display_a() {
//    set_pin(hw_config.r_leds[2], true);

//    set_pin(hw_config.y_leds[0], true);
//    set_pin(hw_config.y_leds[1], true);
//    set_pin(hw_config.y_leds[2], true);
//    set_pin(hw_config.y_leds[3], true);
//    set_pin(hw_config.y_leds[4], true);
}

// Sets LED pins so that they display letter B in morse code
void display_b() {
//    set_pin(hw_config.g_leds[0], true);
//    set_pin(hw_config.g_leds[1], true);
//    set_pin(hw_config.g_leds[2], true);
//    set_pin(hw_config.g_leds[3], true);
//    set_pin(hw_config.g_leds[4], true);

//    set_pin(hw_config.b_leds[0], true);
//    set_pin(hw_config.b_leds[2], true);
//    set_pin(hw_config.b_leds[4], true);
}

// Resets each pile to OFF
void reset() {
//    reset_pile(hw_config.r_leds);
//    reset_pile(hw_config.g_leds);
//    reset_pile(hw_config.y_leds);
//    reset_pile(hw_config.b_leds);
}

// Resets each LED in the pile to OFF
void reset_pile(int *pile) {
//    for (int i = 0; i < LEDS_COUNT; i++) {
//        set_pin(pile[i], false);
//    }
}

// Delays the execution by specified time
void delay_ms(uint32_t ms) {
//    uint32_t start = get_system_time_ms();
//    while (get_system_time_ms() - start < ms) {
//    }
}

uint32_t get_system_time_ms(void) {
    return 0; //  (*SYSTMR_CLO) / 1000; 
}



