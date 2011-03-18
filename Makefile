

XLIB=0
COCOA=1

SRC_C=$(wildcard src/*.c)
SRC_M=$(wildcard src/*.m)

OBJ=$(SRC_C:.c=.o)
ifeq ($(COCOA),1)
OBJ+=  $(SRC_M:.m=.o)
endif

SRC_TEST=test.c
OBJ_TEST=$(SRC_TEST:.c=.o)

CPPFLAGS+=-Iinclude -Wall -Wextra -pedantic
%.c.%.o: CPPFLAGS+=-std=c99


ifeq ($(XLIB),1)
LDFLAGS+= -lX11
endif
ifeq ($(COCOA),1)
LDFLAGS+= -framework Cocoa
endif

run: test
	./test

test: $(OBJ) $(OBJ_TEST)
	$(LINK.c) -o $@ $^



clean:
	$(RM) $(OBJ) $(OBJ_TEST) test



