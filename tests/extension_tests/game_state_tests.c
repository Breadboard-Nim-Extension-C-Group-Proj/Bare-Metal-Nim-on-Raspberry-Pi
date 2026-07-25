#include <stdio.h>
#include "nim.h"
#include "testutil.h"

void test_init_game_1 (void) {
	// game should not be over.
	testcond(!game_state.is_game_over, "Game is NOT over when initialised");
}

void test_init_game_2(void) {
	// every pile should have between 1 and 5 counters.
	bool valid_piles = true;
	int g = get_pile_count(&game_state, G);
	int b = get_pile_count(&game_state, B);
	int r = get_pile_count(&game_state, R);
	int y = get_pile_count(&game_state, Y);
	valid_piles = valid_piles && (1 <= g && g <= 5);
	valid_piles = valid_piles && (1 <= b && b <= 5);
	valid_piles = valid_piles && (1 <= r && r <= 5);
	valid_piles = valid_piles && (1 <= y && y <= 5);
	testcond(valid_piles, "Every pile starts between 1 and 5:");
}

void test_init_game_3(void) {
	
	testcond(game_state.player == PlayerA, "Player A moves first");
}

void test_change_pile_1(void) {
	// standard case
	// init piles:
	game_state.r_pile = 4;
	game_state.g_pile = 4;
	game_state.y_pile = 4;
	game_state.b_pile = 4;

	// init current move:
	game_state.move.pile = 0;
	game_state.move.num_to_remove = 1;

	// call change pile
	change_pile(&game_state);

	testcond(game_state.move.pile == 1, "Change Pile standard case");
}

void test_change_pile_2(void) {
	// next pile is empty
	game_state.r_pile = 4;
	game_state.g_pile = 0;
	game_state.y_pile = 4;
	game_state.b_pile = 4;

	// init current move:
	game_state.move.pile = 0;
	game_state.move.num_to_remove = 1;

	// call change pile
	change_pile(&game_state);

	testcond(game_state.move.pile == 2, "Skips over empty pile");
}

void test_change_pile_3(void) {
	// next 2 piles are empty
	game_state.r_pile = 4;
	game_state.g_pile = 0;
	game_state.y_pile = 0;
	game_state.b_pile = 4;

	// init current move:
	game_state.move.pile = 0;
	game_state.move.num_to_remove = 1;
	
	// call change pile
	change_pile(&game_state);	
	
	testcond(game_state.move.pile == 3, "Skips over 2 empty piles:");
	// next 3 piles are empty
	game_state.r_pile = 0;
	
	change_pile(&game_state);

	testcond(game_state.move.pile == 3, "Skips over 3 empty piles:");
}

void test_change_pile_4(void) {
	// does NOT enter infinite loop if the piles are all empty.
	// exact behaviour may be considered undefined.
	game_state.r_pile = 0;
	game_state.g_pile = 0;
	game_state.y_pile = 0;
	game_state.b_pile = 0;

	game_state.move.pile = 0;
	game_state.move.num_to_remove = 1;

	change_pile(&game_state);
	testcond(true, "Does not enter infinite loop if all piles empty");
}

void test_change_number_1(void) {
	game_state.r_pile = 4;
	game_state.g_pile = 0;
	game_state.y_pile = 0;
	game_state.b_pile = 0;

	game_state.move.pile = 0;
	game_state.move.num_to_remove = 1;

	bool correctCycle = true;
	
	// 1 -> 2
	change_num(&game_state);
	correctCycle = correctCycle && game_state.move.num_to_remove == 2;

	// 2 -> 3
	change_num(&game_state);
	correctCycle = correctCycle && game_state.move.num_to_remove == 3;
	
	// 3 -> 4
	change_num(&game_state);
	correctCycle = correctCycle && game_state.move.num_to_remove == 4;

	// 4 -> 1
	change_num(&game_state);
	correctCycle = correctCycle && game_state.move.num_to_remove == 1;

	testcond(correctCycle, "change_num correctly cycles over pile of 4");

}

void test_change_number_2(void) {
	game_state.r_pile = 1;

	game_state.move.pile = 0;
	game_state.move.num_to_remove = 1;

	change_num(&game_state);
	testcond(game_state.move.num_to_remove == 1, "change_num correctly cycles over pile of 1");
}

void test_apply_move_1(void) {
	// intial state
	game_state.r_pile = 4;
	game_state.g_pile = 4;
	game_state.y_pile = 5;
	game_state.b_pile = 2;
	game_state.player = PlayerA;
	game_state.is_game_over = false;
	game_state.blinking_state = false;
	game_state.move.pile = R;
	game_state.move.num_to_remove = 1;



	// simulate reasonable sequence of button presses:
	//
	change_num(&game_state);
	change_pile(&game_state);
	change_num(&game_state);
	change_num(&game_state);
	// so, we expect that the current move is to remove 3 from g_pile
	
	apply_move(&game_state);
	// we check that the player has changed and the game is not over.
	testcond(!game_state.is_game_over, "Game does not incorrectly end");
	testcond(game_state.player == PlayerB, "Player changes after move is applied");

	// check correct amount has been taken from g_pile.
	testcond(game_state.g_pile == 1, "Move applied correctly");

	// suppose following player wants to take the rest from g_pile.
	while (game_state.move.pile != G) {
		change_pile(&game_state);
	}

	// g_pile has 1, so these shouldn't do anything:
	change_num(&game_state);
	change_num(&game_state);

	apply_move(&game_state);
	testcond(!game_state.is_game_over, "Game does not end when 1 pile empties");
	testcond(game_state.player == PlayerA, "Player changes back after another move applied");
	testcond(game_state.g_pile == 0, "Move applied correctly");
	testcond(game_state.move.pile != G, "Current move does not select empty pile");

}

void test_apply_move_2(void) {
	// initial state (technically illegal as some are empty)
	game_state.r_pile = 4;
	game_state.g_pile = 0;
	game_state.y_pile = 0;
	game_state.b_pile = 2;
	game_state.player = PlayerA;
	game_state.is_game_over = false;
	game_state.blinking_state = false;
	game_state.move.pile = R;
	game_state.move.num_to_remove = 1;

	// Player A moves to remove R pile.
	change_num(&game_state);
	change_num(&game_state);
	change_num(&game_state);
	apply_move(&game_state);
	testcond(!game_state.is_game_over, "Game does not end when 1 pile remains");
	testcond(game_state.player == PlayerB, "Player changes when move applied");
	testcond(game_state.r_pile == 0, "Move applied correctly");
	testcond(game_state.move.pile == B, "Last remaining pile automatically selected");

	// Player B moves to remove rest of G pile (this move should lead to loss).
	// pile change should do nothing
	change_pile(&game_state);

	change_num(&game_state);
	apply_move(&game_state);
	testcond(game_state.is_game_over, "Game ends when last pile depletes");
	testcond(game_state.player == PlayerA, "Current player is the winning player");
	testcond(game_state.b_pile == 0, "Pile emptied");


}

void test_apply_move_3(void) {

}

int main(void) {

	printf("Test Compilation Successful.\n");

	init_game(&game_state); // initialises the values in  game state structure.

	printf("\nGame State Initialisation Tests: \n");
	test_init_game_1();
	test_init_game_2();
	test_init_game_3();

	printf("\nTesting Change Pile: \n");
	test_change_pile_1();
	test_change_pile_2();
	test_change_pile_3();
	test_change_pile_4();

	printf("\nTesting Change Number: \n");
	test_change_number_1();
	test_change_number_2();

	printf("\nTesting Apply Move: \n");	
	test_apply_move_1();
	test_apply_move_2();
	test_apply_move_3();

	printf("\nEnd of test Suite\n");
	return 0;
}
