
#include "ngi/ngi_config.h"

#ifdef NGI_CONTEXT_GLX
#include "ngi/ngi.h"

#include <GL/glx.h>


#include <stdio.h>


int ngi_context_glx_init_1_3(ngi_context* ctx, ngi_window* win) {
    Display* dpy = win->app->plat.xlib.dpy;

    int attr[]= {
      GLX_X_RENDERABLE    , True,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,

        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER,  True,

        GLX_RED_SIZE        , 8,  
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,

        None
    };

    

    int nconfig = 0;
    GLXFBConfig* config = glXChooseFBConfig(dpy, DefaultScreen(dpy), attr, &nconfig);    

    
    GLXContext glxctx = glXCreateNewContext(dpy, config[0], GLX_RGBA_TYPE, NULL, True);
    if(glxctx == NULL) return 0;
    
    GLXWindow glxwin = glXCreateWindow(dpy, config[0], (Window)win->plat.xlib.win, NULL);


    glXMakeContextCurrent(dpy, glxwin, glxwin, glxctx);

    ctx->platform.glx.ctx = glxctx;
    ctx->platform.glx.drawable = glxwin;
    
    return 1;
}

int ngi_context_glx_init_1_0(ngi_context* ctx, ngi_window* win) {
    Display* dpy = win->app->plat.xlib.dpy;
    XVisualInfo* xvi;
    GLXContext glxctx;
    Window xwin = (Window)win->plat.xlib.win;
    
    int attr[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,  
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        None    
    };
    
    xvi = glXChooseVisual( dpy, DefaultScreen(dpy), attr );

    if(xvi == NULL) return 0;
    
    glxctx =  glXCreateContext(dpy,xvi, NULL, True);

    if(glxctx == NULL) return 0;
    
    ctx->platform.glx.ctx = glxctx;
    ctx->platform.glx.drawable = xwin;
    
    glXMakeCurrent(dpy, xwin, glxctx);

    
    return 1;
}

int ngi_context_glx_init(ngi_context* ctx, ngi_window* win) {

    int minorVersion = 0, majorVersion = 0;
    int ret; 
    Display* dpy = win->app->plat.xlib.dpy;
    
    
    
    ret = glXQueryVersion(dpy, &majorVersion, &minorVersion);

    printf("glx version %d.%d\n",majorVersion, minorVersion);

    if(ret == False) return 0;
    if(win->app->type != ngi_wm_api_xlib) return 0;

    ctx->type = ngi_context_api_glx;
    ctx->graphics = ngi_graphics_api_opengl;
    ctx->app = win->app;
    

    if(majorVersion == 1 && minorVersion == 3)
        ret = ngi_context_glx_init_1_3(ctx, win);
    else 
        ret = ngi_context_glx_init_1_0(ctx, win);

    if(ret == 0) return 0;

    win->context = ctx;

    return 1;
}

int ngi_context_glx_swap(ngi_context* ctx) {

    Display *dpy = ctx->app->plat.xlib.dpy;
//    GLXContext glxctx = ctx->platform.glx.ctx;
    GLXWindow glxwin = ctx->platform.glx.drawable;

//    glXMakeContextCurrent(dpy, glxwin, glxwin, glxctx);

    
    glXSwapBuffers(dpy, glxwin);
    return 1;
}


#endif

extern int dummy;
