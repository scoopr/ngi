
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

$(eval $(call AUTOCONF_TEST,EGL, -lEGL -DTEST_EGL ))
$(eval $(call AUTOCONF_TEST,GLES1, -lGLESv1_CM  -DTEST_GLES1 ))
$(eval $(call AUTOCONF_TEST,GLES2, -lGLESv2  -DTEST_GLES2 ))
$(eval $(call AUTOCONF_TEST,OSX, -framework Cocoa -DTEST_OSX_OPENGL  ))
$(eval $(call AUTOCONF_TEST,OPENGL, -lGL -DTEST_OPENGL  ))
$(eval $(call AUTOCONF_TEST,XLIB_A, -L/usr/lib/X11 -lX11 -DTEST_XLIB  ))
$(eval $(call AUTOCONF_TEST,XLIB_B, -L/usr/X11/lib -lX11 -DTEST_XLIB  ))
$(eval $(call AUTOCONF_TEST,XCB, -lxcb -DTEST_XCB  ))
$(eval $(call AUTOCONF_TEST,GLX, -I/usr/X11R6/include   -DTEST_GLX   ))
$(shell $(RM) .testbin)

endif


include $(CACHE_FILE)
XLIB=$(XLIB_A)$(XLIB_B)

$(info [ngi conf] EGL:$(EGL) GLES1:$(GLES1) GLES2:$(GLES2) OPENGL:$(OPENGL) OSX:$(OSX) XLIB:$(XLIB) XCB:$(XCB) GLX:$(GLX))


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

ifeq ($(OPENGL),1)
LDFLAGS+= -lGL
CPPFLAGS+= -DNGI_RENDER_API_OPENGL
endif

ifneq ($(XLIB),0)
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

ifeq ($(GLX),1)
CPPFLAGS+= -DNGI_CONTEXT_GLX -I/usr/X11/include
LDFLAGS+=-lGL
endif




BUILD_DIR=build
OBJ:=$(addprefix $(BUILD_DIR)/,$(OBJ))
OBJ_PROBE:=$(addprefix $(BUILD_DIR)/,$(OBJ_PROBE))
PATH_SEPARATOR=/
MKDIR=mkdir -p

# Suppres default rules
%.o: %.c ;
%.o: %.m ;


FILE_DEP_DIR=$(BUILD_DIR)/deps
FILE_DEPS:=$(SRC_C:.c=.d) $(SRC_PROBE:.c=.d) $(SRC_M:.m=.d)
FILE_DEPS:=$(addprefix $(FILE_DEP_DIR)/,$(FILE_DEPS))


DEP_FLAGS=-E -w -Wno-unused-parameter -MMD -MP 

# deps
$(FILE_DEP_DIR)/%.d: %.c
	@$(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	@$(COMPILE.c) $(CPPFLAGS) $(CFLAGS) $(DEP_FLAGS) -MT $(BUILD_DIR)/$(<:.c=.o) $< -MF $@ 2>&1 > /dev/null

$(FILE_DEP_DIR)/%.d: %.m
	@$(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	@$(COMPILE.m) $(CPPFLAGS) $(CXXFLAGS) $(DEP_FLAGS) -MT $(BUILD_DIR)/$(<:.m=.o) $< -MF $@ 2>&1 > /dev/null



# compiles
$(BUILD_DIR)/%.o: %.c
	@echo CC $<
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.c) -o $@ $<

$(BUILD_DIR)/%.o: %.m
	@echo CM $<
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.m) -o $@ $<









probe/probe: $(OBJ) $(OBJ_PROBE)
	$(LINK.c) -o $@ $^



clean:
	$(RM) $(OBJ) $(OBJ_PROBE) probe/probe



$(BUILD_DIR)/probe/typo.o: CPPFLAGS+=-Wno-unused-parameter
$(BUILD_DIR)/probe/typo.o: CPPFLAGS+=-Wno-unused-variable
$(BUILD_DIR)/src/ngi_window_cocoa.o: CPPFLAGS+=-Wno-overflow

ifeq ($(CC),clang)
$(BUILD_DIR)/probe/typo.o: CPPFLAGS+=-Wno-array-bounds
endif


ifneq ($(SKIP_FILE_DEPS),)
-include $(FILE_DEPS)
endif
