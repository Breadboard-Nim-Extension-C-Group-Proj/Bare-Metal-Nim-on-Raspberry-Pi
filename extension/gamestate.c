// This file contains functions for the game logic 

#include "nim.h"

// Returns the number of objects available in a specified pile
int get_pile_count(GameState* state, Pile p) {
    switch(p) {
        case G: return state->g_pile;
        case B: return state->b_pile;
        case R: return state->r_pile;
        case Y: return state->y_pile;
    }
    return 0;
}

// Returns pin numbers for a specified pile 
int *get_pin_state(PinsState* state, Pile p){
    switch(p) {
        case G: return state->g_leds;
        case B: return state->b_leds;
        case R: return state->r_leds;
        case Y: return state->y_leds;
    }
    return 0;
}

// Checks if the game is finished 
static void check_win_condition(GameState* state) {
    // If each pile has 0 objects left, game is finished 
    if (state->g_pile == 0 && state->b_pile == 0 && 
        state->r_pile == 0 && state->y_pile == 0) {
        state->is_game_over = true;
    }
}

// Initialises GameState
void init_game(GameState* state) {
    // First player is PlayerA
    state->player = PlayerA;

    // Each pile is randomly generate to contain 2, 4, or 5 objects 
    state->g_pile = get_rand();
    state->b_pile = get_rand();
    state->r_pile = get_rand();
    state->y_pile = get_rand();

    // Flag is_game_over is set to false, and blinking_state is ON 
    state->is_game_over = false;
    state->blinking_state = ON;

    // Initialise first move to Red pile with 1 LED to remove 
    state->move.pile = R;
    state->move.num_to_remove = 1;
}

// This function is executed when the game is finished 
void end_game(GameState* state, int *last_debounce_time, bool *last_debounce_state) {
    CurrentPlayer player = state->player;   // The player that won

    reset();    // Reset the LEDs 

    // Variables that keep track of blinking delat
    int blink_time = 0; 
    bool to_show = false; 

    // While button is not pressed, output the endgame state 
    while (!check_button_press(hw_config.button_confirm, last_debounce_time, last_debounce_state)) {
        int current_time = get_system_time_ms(); 
        
        // This if-block is only executed when some time has elapsed from the last time LEDs changed state 
        if (current_time - blink_time > BLINK_INTERVAL) {

            // This block implements manual blinking of the winner letter 
            blink_time = current_time; 
            if (to_show) {
                // Reset the board
                reset(); 
                to_show = false; 
            } else {
                // Show the corresponding letter for the winner
                if (player == PlayerA) {
                    display_a(); 
                } else {
                    display_b(); 
                }
                to_show = true; 
            }
        }
    }

    reset();    // Reset the board again before restarting the game 
}

//it not only apply the move but also check if the game ends and call end_game, also it changes player and reset choose in move
void apply_move(GameState* state) {
    // In each case, remove some number of objects from specified pile 
    switch (state->move.pile)
    {
    case G:
        state->g_pile -= state->move.num_to_remove;
        break;
    case B:
        state->b_pile -= state->move.num_to_remove;
        break;
    case R:
        state->r_pile -= state->move.num_to_remove;
        break;
    case Y:
        state->y_pile -= state->move.num_to_remove;
        break;
    default:
        break;
    }

    // Change the player, and check for the win conditions. 
    state->player = 1 - state->player;
    check_win_condition(state);
    state->move.pile = B;
    change_pile(state);
}

// Used when user pushes the "change pile" button
void change_pile(GameState* state){
    Pile pile = state->move.pile;
    pile = (pile + 1) % 4;
    
    while(get_pile_count(state, pile) <= 0){
        pile = (pile + 1) % 4;
        if(pile == state->move.pile){
            break;
        }
    }
    state->move.pile = pile;
    state->move.num_to_remove = 1;
}

// Used when user pushes the "change num" button
void change_num(GameState* state){
    Pile pile = state->move.pile;
    if(state->move.num_to_remove < get_pile_count(state, pile)){
        state->move.num_to_remove++;
    }else{
        state->move.num_to_remove = 1;
    }
}