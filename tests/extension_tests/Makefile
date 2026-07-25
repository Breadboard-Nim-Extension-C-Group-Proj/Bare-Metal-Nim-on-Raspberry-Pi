CC      = gcc
CFLAGS  =  -I../../extension
BUILD	= game_state_tests
all: $(BUILD)

game_state_tests: 	game_state_tests.c \
			testutil.c \
			../../extension/gamestate.c \
			nim_utils.c \

	$(CC) $(CFLAGS) \
		game_state_tests.c \
		testutil.c \
		../../extension/gamestate.c \
		nim_utils.c \
		-o game_state_tests


clean:
	rm -f game_state_tests *.o
