
#ifdef TEST_EGL
#include <EGL/egl.h>
#endif

#ifdef TEST_GLES1
#include <GLES/gl.h>
#endif

#ifdef TEST_GLES2
#include <GLES2/gl2.h>
#endif

#ifdef TEST_OSX_OPENGL
#include <OpenGL/gl.h>
#endif

#ifdef TEST_OPENGL
#include <GL/gl.h>
#endif

#ifdef TEST_XLIB
#include <X11/Xlib.h>
#endif

#ifdef TEST_XCB
#include <xcb/xcb.h>
#endif

#ifdef TEST_WIN32
#include <windows.h>
#endif

#ifdef TEST_GLX
#include <GL/glx.h>
#endif

int main() {
    return 0;
}

