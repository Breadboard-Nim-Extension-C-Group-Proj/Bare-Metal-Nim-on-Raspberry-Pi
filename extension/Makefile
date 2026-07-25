CC ?= gcc
CFLAGS ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic -I../shared

SRCS = \
       gamestate.c \
	   nim_utils.c \
	   nim.c \
	   pins.c
    
OBJS = $(SRCS:.c=.o)

nim: $(OBJS)
	$(CC) $(OBJS) -o nim

clean:
	rm -f $(OBJS) nim
