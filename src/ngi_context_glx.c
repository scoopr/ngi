
#include "ngi/ngi_config.h"

#ifdef NGI_CONTEXT_GLX
#include "ngi/ngi.h"

#include <GL/glx.h>


#include <stdio.h>


int ngi_context_glx_init_1_3(ngi_context* ctx, ngi_window* win, ngi_format* format) {
    Display* dpy = win->app->plat.xlib.dpy;
 
/*    int attr[]= {

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
*/
    
    Window* xwin = win->plat.xlib.win;
    
//    int nconfig = 0;
//    GLXFBConfig* config = glXChooseFBConfig(dpy, DefaultScreen(dpy), attr, &nconfig);    

//    GLXFBConfig fbc = config[0];


    GLXFBConfig fbc = format->platform.xlib.fbc;

    GLXContext glxctx = glXCreateNewContext(dpy, fbc, GLX_RGBA_TYPE, NULL, True);
    if(glxctx == NULL) return 0;
    



    glXMakeCurrent( dpy, (GLXDrawable)xwin, glxctx );

    ctx->platform.glx.ctx = glxctx;
    ctx->platform.glx.drawable = (void*)xwin;
    
    return 1;
}

int ngi_context_glx_init_1_0(ngi_context* ctx, ngi_window* win, ngi_format* format) {
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

    (void)format;    
    xvi = glXChooseVisual( dpy, DefaultScreen(dpy), attr );

    if(xvi == NULL) return 0;
    
    glxctx =  glXCreateContext(dpy,xvi, NULL, True);

    if(glxctx == NULL) return 0;
    
    ctx->platform.glx.ctx = glxctx;
    ctx->platform.glx.drawable = (void*)xwin;
    
    glXMakeCurrent(dpy, xwin, glxctx);

    
    return 1;
}

int ngi_context_glx_init(ngi_context* ctx, ngi_window* win, ngi_format* format) {

    int minorVersion = 0, majorVersion = 0;
    int ret; 
    Display* dpy = win->app->plat.xlib.dpy;
    
    
    
    ret = glXQueryVersion(dpy, &majorVersion, &minorVersion);

//    printf("glx version %d.%d\n",majorVersion, minorVersion);

    if(ret == False) return 0;
    if(win->app->type != ngi_wm_api_xlib) return 0;

    ctx->type = ngi_context_api_glx;
    ctx->graphics = ngi_graphics_api_opengl;
    ctx->app = win->app;
    

    if(majorVersion == 1 && minorVersion >= 3)
        ret = ngi_context_glx_init_1_3(ctx, win, format);
    else 
        ret = ngi_context_glx_init_1_0(ctx, win, format);

    if(ret == 0) return 0;

    win->context = ctx;

    return 1;
}

int ngi_context_glx_swap(ngi_context* ctx) {

    Display *dpy = ctx->app->plat.xlib.dpy;
//    GLXContext glxctx = ctx->platform.glx.ctx;
    GLXWindow glxwin = (GLXDrawable)ctx->platform.glx.drawable;

//    glXMakeContextCurrent(dpy, glxwin, glxwin, glxctx);

    
    glXSwapBuffers(dpy, glxwin);
    return 1;
}

int ngi_context_glx_set_active(ngi_context* ctx) {
    Display *dpy = ctx->app->plat.xlib.dpy;
    GLXContext glxctx = ctx->platform.glx.ctx;
    Window* xwin = ctx->platform.glx.drawable;
    glXMakeCurrent(dpy, (GLXDrawable)xwin, glxctx);
    return 1;
}

#endif

extern int dummy;
