
.DEFAULT: all
all:

SILENT?=@

AUTOCONF=1

ifeq ($(MAKECMDGOALS),clean)
AUTOCONF=0
else
SKIP_FILE_DEPS=1
endif

CACHE_FILE=.config.cache
ifeq ($(wildcard $(CACHE_FILE)),$(CACHE_FILE))
AUTOCONF=0
endif

ifneq ($(FORCE),)
AUTOCONF=1
endif

AUTOCONF_TEST=$$(if $$(shell $$(TEST_CC) $(2) conf.c  2>&1), \
                    $$(shell echo $(1)=0 >> $$(CACHE_FILE)),  \
                    $$(shell echo $(1)=1 >> $$(CACHE_FILE)))


ifeq ($(AUTOCONF),1)
TEST_CC=$(CC) -o .testbin

$(eval $(call AUTOCONF_TEST,HAVE_EGL, -lEGL -DTEST_EGL ))
$(eval $(call AUTOCONF_TEST,HAVE_GLES1, -lGLESv1_CM  -DTEST_GLES1 ))
$(eval $(call AUTOCONF_TEST,HAVE_GLES2, -lGLESv2  -DTEST_GLES2 ))
$(eval $(call AUTOCONF_TEST,HAVE_OSX, -framework Cocoa -DTEST_OSX_OPENGL  ))
$(eval $(call AUTOCONF_TEST,HAVE_OPENGL, -lGL -DTEST_OPENGL  ))
$(eval $(call AUTOCONF_TEST,HAVE_XLIB_A, -L/usr/lib/X11 -lX11 -DTEST_XLIB  ))
$(eval $(call AUTOCONF_TEST,HAVE_XLIB_B, -L/usr/X11/lib -lX11 -DTEST_XLIB  ))
$(eval $(call AUTOCONF_TEST,HAVE_XCB, -lxcb -DTEST_XCB  ))
$(eval $(call AUTOCONF_TEST,HAVE_GLX, -I/usr/X11R6/include   -DTEST_GLX   ))
$(shell $(RM) .testbin)

endif


include $(CACHE_FILE)
ifneq ($(HAVE_XLIB_A)$(HAVE_XLIB_B),00)
HAVE_XLIB=1
else
HAVE_XLIB=0
endif

ifeq ($(VARIANT),)
$(info [ngi conf] EGL:$(HAVE_EGL) GLES1:$(HAVE_GLES1) GLES2:$(HAVE_GLES2) OPENGL:$(HAVE_OPENGL) OSX:$(HAVE_OSX) XLIB:$(HAVE_XLIB) XCB:$(HAVE_XCB) GLX:$(HAVE_GLX))
endif

SRC_C=$(wildcard src/*.c)
SRC_M=$(wildcard src/*.m)

OBJ=$(SRC_C:.c=.o)
ifeq ($(OSX),1)
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


ifneq ($(findstring -gl-,$(VARIANT)),)
LDFLAGS+= -lGL
CPPFLAGS+= -DNGI_RENDER_API_OPENGL
endif

ifneq ($(findstring -xlib-,$(VARIANT)),)
LDFLAGS+= -L/usr/lib/X11 -L/usr/X11/lib -lX11
CPPFLAGS+= -DNGI_WINDOW_XLIB
# -lGL
endif

ifneq ($(findstring -xcb-,$(VARIANT)),)
LDFLAGS+= -lxcb
CPPFLAGS+= -DNGI_WINDOW_XCB
# -lGL
endif

ifneq ($(findstring -egl-,$(VARIANT)),)
LDFLAGS+= -lEGL
CPPFLAGS+= -DNGI_CONTEXT_EGL
endif

ifneq ($(findstring -gles1-,$(VARIANT)),)
LDFLAGS+= -lGLESv1_CM
CPPFLAGS+= -DNGI_RENDER_API_GLES1
endif

ifneq ($(findstring -gles2-,$(VARIANT)),)
LDFLAGS+= -lGLESv2
CPPFLAGS+= -DNGI_RENDER_API_GLES2
endif

ifneq ($(findstring -osx-,$(VARIANT)),)
LDFLAGS+= -framework Cocoa -framework OpenGL -liconv
CPPFLAGS+= -DNGI_RENDER_API_OPENGL -DNGI_CONTEXT_COCOA -DNGI_WINDOW_COCOA
.PHONY: resources
resources: ngi.nib
probe/probe: | resources
ngi.nib: ngi.xib
	ibtool --compile ngi.nib ngi.xib

OBJ+=  $(SRC_M:.m=.o)

endif

ifneq ($(findstring -glx-,$(VARIANT)),)
CPPFLAGS+= -DNGI_CONTEXT_GLX -I/usr/X11/include
LDFLAGS+=-lGL
endif

ifeq ($(shell uname),Linux)
LDFLAGS+=-lrt
endif

EXE=probe/probe$(VARIANT)


ifeq ($(VARIANT),)

VARIANT_LIST=

ifeq ($(HAVE_OSX),1)
VARIANT_LIST+= -osx
endif
ifeq ($(HAVE_OSX)$(HAVE_GLX),11)
VARIANT_LIST+= -xlib-glx-gl
endif
ifeq ($(HAVE_XLIB)$(HAVE_GLX)$(HAVE_OPENGL),111)
VARIANT_LIST+= -xlib-glx-gl
endif
ifeq ($(HAVE_XLIB)$(HAVE_EGL)$(HAVE_OPENGL),111)
VARIANT_LIST+= -xlib-egl-gl
endif
ifeq ($(HAVE_XLIB)$(HAVE_EGL)$(HAVE_GLES1),111)
VARIANT_LIST+= -xlib-egl-gles1
endif
ifeq ($(HAVE_XLIB)$(HAVE_EGL)$(HAVE_GLES2),111)
VARIANT_LIST+= -xlib-egl-gles2
endif
ifeq ($(HAVE_XLIB)$(HAVE_EGL)$(HAVE_GLES1)$(HAVE_GLES2),1111)
VARIANT_LIST+= -xlib-egl-gles1-gles2
endif


define BUILD_VARIANT_TEMPLATE
$(SILENT) $(MAKE) VARIANT=$(var)

endef


all:
	$(foreach var,$(VARIANT_LIST),$(BUILD_VARIANT_TEMPLATE))


else

all: $(EXE)

endif



run: $(EXE)
	./$(EXE)

.PHONY: lib
lib: lib/libngi$(VARIANT).a
lib/libngi$(VARIANT).a: $(OBJ)
	$(MKDIR) lib
	$(AR) rcs $@ $^


BUILD_DIR=build/obj$(VARIANT)
OBJ:=$(addprefix $(BUILD_DIR)/,$(OBJ))
OBJ_PROBE:=$(addprefix $(BUILD_DIR)/,$(OBJ_PROBE))
PATH_SEPARATOR=/
MKDIR=mkdir -p

# Suppres default rules
%.o: %.c ;
%.o: %.m ;


FILE_DEP_DIR=$(BUILD_DIR)/deps
FILE_DEPS:=$(SRC_C:.c=.d) $(SRC_PROBE:.c=.d) 
ifeq ($(HAVE_OSX),1)
FILE_DEPS+= $(SRC_M:.m=.d)
endif
FILE_DEPS:=$(addprefix $(FILE_DEP_DIR)/,$(FILE_DEPS))


DEP_FLAGS=-E -w -Wno-unused-parameter -MMD -MP 

# deps
$(FILE_DEP_DIR)/%.d: %.c
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.c) $(CPPFLAGS) $(CFLAGS) $(DEP_FLAGS) -MT $(BUILD_DIR)/$(<:.c=.o) $< -MF $@ 2>&1 > /dev/null

$(FILE_DEP_DIR)/%.d: %.m
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.m) $(CPPFLAGS) $(CXXFLAGS) $(DEP_FLAGS) -MT $(BUILD_DIR)/$(<:.m=.o) $< -MF $@ 2>&1 > /dev/null



# compiles
$(BUILD_DIR)/%.o: %.c
	@echo CC $<
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.c) -o $@ $<

$(BUILD_DIR)/%.o: %.m
	@echo CM $<
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.m) -o $@ $<









$(EXE): $(OBJ) $(OBJ_PROBE)
	$(CC) $^ $(LDFLAGS) -o $@ 



clean:
	$(RM) -r  build .config.cache



$(BUILD_DIR)/probe/typo.o: CPPFLAGS+=-w
$(BUILD_DIR)/src/ngi_window_cocoa.o: CPPFLAGS+=-Wno-overflow

ifeq ($(CC),clang)
$(BUILD_DIR)/probe/typo.o: CPPFLAGS+=-Wno-array-bounds
endif


ifneq ($(SKIP_FILE_DEPS),)
-include $(FILE_DEPS)
endif
