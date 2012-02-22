#include "ngi/ngi.h"
#include <GL/glx.h>

int ngi_format_init_glx(ngi_application* app, ngi_format* format, ngi_config *config)
{
    (void)app;
    (void)config;

    int attr[]= {

        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER,  True,

        GLX_RED_SIZE        , 8,  
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 0,
        GLX_DEPTH_SIZE      , 24,

        None
    };

    Display* dpy = app->plat.xlib.dpy;
    
    int nfbconfig = 0;
    GLXFBConfig* fbconfig = glXChooseFBConfig(dpy, DefaultScreen(dpy), attr, &nfbconfig);    


    if(nfbconfig == 0) return 0;
    
    GLXFBConfig fbc = fbconfig[0];

    
    format->platform.xlib.fbc = fbc;
    format->platform.xlib.visual = glXGetVisualFromFBConfig(dpy, fbc);
    
    return 1;
    
}

