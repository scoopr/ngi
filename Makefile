
AUTOCONF=1

ifeq ($(MAKECMDGOALS),clean)
AUTOCONF=0
endif


ifeq ($(AUTOCONF),1)
TEST_CC=$(CC) -o .testbin
ifeq ($(shell $(TEST_CC) -lEGL conf.c -DTEST_EGL 2>&1),)
EGL=1
else
EGL=0
endif

ifeq ($(shell $(TEST_CC) -lGLESv1_CM conf.c -DTEST_GLES1 2>&1),)
GLES1=1
else
GLES1=0
endif

ifeq ($(shell $(TEST_CC) -lGLESv2 conf.c -DTEST_GLES2 2>&1),)
GLES2=1
else
GLES2=0
endif

ifeq ($(shell $(TEST_CC) -framework Cocoa conf.c -DTEST_OSX_OPENGL 2>&1),)
OSX=1
else
OSX=0
endif

ifeq ($(shell $(TEST_CC) -lGL conf.c -DTEST_OPENGL 2>&1),)
OPENGL=1
else
OPENGL=0
endif

ifeq ($(shell $(TEST_CC) -L/usr/lib/X11 -L/usr/X11/lib -lX11 conf.c -DTEST_XLIB ),)
XLIB=1
else
XLIB=0
endif

ifeq ($(shell $(TEST_CC) -lxcb conf.c -DTEST_XCB 2>&1),)
XCB=1
else
XCB=0
endif

$(shell $(RM) .testbin)
$(info [ngi conf] EGL:$(EGL) GLES1:$(GLES1) GLES2:$(GLES2) OPENGL:$(OPENGL) OSX:$(OSX) XLIB:$(XLIB) XCB:$(XCB))

endif





SRC_C=$(wildcard src/*.c)
SRC_M=$(wildcard src/*.m)

OBJ=$(SRC_C:.c=.o)
ifeq ($(OSX),1)
OBJ+=  $(SRC_M:.m=.o)
endif

ifeq ($(MAKECMDGOALS),clean)
OBJ+=  $(SRC_M:.m=.o)
endif

SRC_PROBE=$(wildcard probe/*.c)
OBJ_PROBE=$(SRC_PROBE:.c=.o)

CPPFLAGS+=-Iinclude -Wall -Wextra -pedantic -ggdb -Werror
# -arch i386
#LDFLAGS+=-arch i386
CFLAGS+=-std=c99


ifeq ($(XLIB),1)
LDFLAGS+= -L/usr/lib/X11 -L/usr/X11/lib -lX11
CPPFLAGS+= -DNGI_WINDOW_XLIB
# -lGL
endif

ifeq ($(XCB),1)
LDFLAGS+= -lxcb
CPPFLAGS+= -DNGI_WINDOW_XCB
# -lGL
endif

ifeq ($(EGL),1)
LDFLAGS+= -lEGL
CPPFLAGS+= -DNGI_CONTEXT_EGL
endif

ifeq ($(GLES1),1)
LDFLAGS+= -lGLESv1_CM
CPPFLAGS+= -DNGI_RENDER_API_GLES1
endif

ifeq ($(GLES2),1)
LDFLAGS+= -lGLESv2
CPPFLAGS+= -DNGI_RENDER_API_GLES2
endif

ifeq ($(OPENGL),1)
LDFLAGS+= -lGL
CPPFLAGS+= -DNGI_RENDER_API_OPENGL
endif

run: probe/probe
	./probe/probe

ifeq ($(OSX),1)
LDFLAGS+= -framework Cocoa -framework OpenGL -liconv
CPPFLAGS+= -DNGI_RENDER_API_OPENGL -DNGI_CONTEXT_COCOA -DNGI_WINDOW_COCOA
.PHONY: resources
resources: ngi.nib
probe/probe: | resources
ngi.nib: ngi.xib
	ibtool --compile ngi.nib ngi.xib
endif


probe/probe: $(OBJ) $(OBJ_PROBE)
	$(LINK.c) -o $@ $^



clean:
	$(RM) $(OBJ) $(OBJ_PROBE) probe/probe



probe/typo.o: CPPFLAGS+=-Wno-unused-parameter
probe/typo.o: CPPFLAGS+=-Wno-unused-variable

ifeq ($(CC),clang)
probe/typo.o: CPPFLAGS+=-Wno-array-bounds
endif
