

XLIB=1

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

SRC_TEST=test.c
OBJ_TEST=$(SRC_TEST:.c=.o)

CPPFLAGS+=-Iinclude -Wall -Wextra -pedantic -std=c99

ifeq ($(XLIB),1)
LDFLAGS+= -lX11
endif

run: test
	./test

test: $(OBJ) $(OBJ_TEST)
	$(LINK.c) -o $@ $^



clean:
	$(RM) $(OBJ) $(OBJ_TEST) test



