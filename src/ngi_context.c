
#include "ngi/ngi.h"

int ngi_context_init(ngi_context* ctx, ngi_window* win, ngi_format* format) {
    
    #ifdef NGI_CONTEXT_GLX
    if(win->app->type == ngi_wm_api_xlib)
        return ngi_context_glx_init(ctx, win);
    #endif
    
    #ifdef NGI_CONTEXT_EGL
    if(win->app->type == ngi_wm_api_xlib)
        return ngi_context_egl_init(ctx, win);
    #endif

    #ifdef NGI_CONTEXT_COCOA
    if(win->app->type == ngi_wm_api_cocoa)
        return ngi_context_cocoa_init(ctx, win, format);
    #endif

    #ifdef NGI_CONTEXT_IOS
    if(win->app->type == ngi_wm_api_ios)
        return ngi_context_ios_init(ctx, win);
    #endif
    
    #ifdef NGI_CONTEXT_WGL
    if(win->app->type == ngi_wm_api_win32)
        return ngi_context_wgl_init(ctx, win);
    #endif
    
    return 0;

}

int ngi_context_swap(ngi_context* ctx) {

    #ifdef NGI_CONTEXT_GLX
    if(ctx->type == ngi_context_api_glx)
        return ngi_context_glx_swap(ctx);
    #endif

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

    #ifdef NGI_CONTEXT_IOS
    if(ctx->type == ngi_context_api_ios)
        return ngi_context_ios_swap(ctx);
    #endif
    
    return 0;
    
}

int ngi_context_set_active(ngi_context* ctx) {

    #ifdef NGI_CONTEXT_GLX
    if(ctx->type == ngi_context_api_glx)
        return ngi_context_glx_set_active(ctx);
    #endif

    #ifdef NGI_CONTEXT_EGL
    if(ctx->type == ngi_context_api_egl)
        return ngi_context_egl_set_active(ctx);
    #endif

    #ifdef NGI_CONTEXT_COCOA
    if(ctx->type == ngi_context_api_cocoa)
        return ngi_context_cocoa_set_active(ctx);
    #endif
    
    #ifdef NGI_CONTEXT_WGL
    if(ctx->type == ngi_context_api_wgl)
        return ngi_context_wgl_set_active(ctx);
    #endif

    #ifdef NGI_CONTEXT_IOS
    if(ctx->type == ngi_context_api_ios)
        return ngi_context_ios_set_active(ctx);
    #endif
    
    return 0;
    
}

