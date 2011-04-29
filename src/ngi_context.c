
#include "ngi/ngi.h"

int ngi_context_init(ngi_context* ctx, ngi_window* win) {
    
    
    #ifdef NGI_CONTEXT_EGL
    if(win->app->type == ngi_wm_api_xlib)
        return ngi_context_egl_init(ctx, win);
    #endif

    #ifdef NGI_CONTEXT_COCOA
    if(win->app->type == ngi_wm_api_cocoa)
        return ngi_context_cocoa_init(ctx, win);
    #endif
    
    #ifdef NGI_CONTEXT_WGL
    if(win->app->type == ngi_wm_api_win32)
        return ngi_context_wgl_init(ctx, win);
    #endif
    
    return 0;

}

int ngi_context_swap(ngi_context* ctx) {

    #ifdef NGI_CONTEXT_EGL
    if(ctx->type == ngi_context_api_egl)
        return ngi_context_egl_swap(ctx);
    #endif

    #ifdef NGI_CONTEXT_COCOA
    if(ctx->type == ngi_context_api_cocoa)
        return ngi_context_cocoa_swap(ctx);
    #endif
    
    #ifdef NGI_CONTEXT_WGL
    if(ctx->type == ngi_context_api_wgl)
        return ngi_context_wgl_swap(ctx);
    #endif
    
    return 0;
    
}

