
ifeq ($(shell uname -s),Darwin)
COCOA=1
endif

ifeq ($(shell uname -s),Linux)
XLIB=1
EGL=1
GLES2=1
endif


SRC_C=$(wildcard src/*.c)
SRC_M=$(wildcard src/*.m)

OBJ=$(SRC_C:.c=.o)
ifeq ($(COCOA),1)
OBJ+=  $(SRC_M:.m=.o)
endif

SRC_TEST=test.c
OBJ_TEST=$(SRC_TEST:.c=.o)

CPPFLAGS+=-Iinclude -Wall -Wextra -pedantic
CFLAGS+=-std=c99

ifeq ($(XLIB),1)
LDFLAGS+= -lX11 -lEGL
endif
ifeq ($(EGL),1)
LDFLAGS+= -lEGL
CPPFLAGS+= -DNGI_CONTEXT_EGL
endif
ifeq ($(GLES2),1)
LDFLAGS+= -lGLESv2
endif

ifeq ($(COCOA),1)
LDFLAGS+= -framework Cocoa -framework OpenGL
endif

run: test
	./test

test: $(OBJ) $(OBJ_TEST)
	$(LINK.c) -o $@ $^



clean:
	$(RM) $(OBJ) $(OBJ_TEST) test



