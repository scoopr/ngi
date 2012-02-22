
#include "ngi/ngi.h"

#include <assert.h>

static inline const char* find_config_string(const char* name, ngi_config* config, const char* fallback) {

    int i;
    
    if(config == 0) return 0;

    for(i = 0; i < config->num_entries; ++i)
    {
        if(config->config[i].name == name) return config->config[i].value.s;
    }

    return fallback;

}

static inline int find_config_int(const char* name, ngi_config* config, int fallback) {

    int i;
    
    if(config == 0) return 0;

    for(i = 0; i < config->num_entries; ++i)
    {
        if(config->config[i].name == name) return config->config[i].value.i;
    }

    return fallback;

}

int ngi_window_init(ngi_application* app, ngi_window* win, ngi_format* format, ngi_config* config) {
    
    const char* wmapi = app->type; //find_config_string(ngi_config_wm_api, config, NULL);

    (void)config;
    win->app = app;

    // const char* ctxapi = find_config_string(ngi_config_context_api, config, NULL);
    // // ngi_graphics_api gfxapi = find_config(ngi_config_graphics_api, config);
    // // 
    // // 
    // if( ctxapi == NULL ) {
    //     
    //     #ifdef NGI_CONTEXT_EGL
    //     ctxapi = ngi_context_api_egl;
    //     #endif
    // 
    //     #ifdef NGI_CONTEXT_GLX
    //     ctxapi = ngi_context_api_glx;
    //     #endif
    // 
    //     #ifdef NGI_CONTEXT_COCOA
    //     ctxapi = ngi_context_api_cocoa;
    //     #endif
    // 
    //     #ifdef NGI_CONTEXT_WGL
    //     ctxapi = ngi_context_api_wgl;
    //     #endif
    //     
    //     if(ctxapi == NULL) return 0;
    //     
    // }

    if( wmapi == NULL ) {

        #ifdef NGI_WINDOW_XLIB
        wmapi = ngi_wm_api_xlib;
        #endif
        #ifdef NGI_WINDOW_WIN32
        wmapi = ngi_wm_api_win32;
        #endif
        #ifdef NGI_WINDOW_COCOA
        wmapi = ngi_wm_api_cocoa;
        #endif
        #ifdef NGI_WINDOW_IOS
        wmapi = ngi_wm_api_ios;
        #endif
        

        if(wmapi == NULL) return 0;
        
    }
    
    #ifdef NGI_WINDOW_XLIB
    if(ngi_config_match(wmapi, ngi_wm_api_xlib)) return ngi_window_init_xlib(app, win);
    #endif
    
    #ifdef NGI_WINDOW_WIN32
    if(ngi_config_match(wmapi, ngi_wm_api_win32)) return ngi_window_init_win32(app, win);
    #endif

    #ifdef NGI_WINDOW_COCOA
    if(ngi_config_match(wmapi, ngi_wm_api_cocoa)) return ngi_window_init_cocoa(app, win, format);
    #endif

    #ifdef NGI_WINDOW_IOS
    if(ngi_config_match(wmapi, ngi_wm_api_ios)) return ngi_window_init_ios(app, win);
    #endif

    return 0;
}

int ngi_window_deinit(ngi_window* win) {

    #ifdef NGI_WINDOW_XLIB
    if(win->app->type == ngi_wm_api_xlib) {
        return ngi_window_deinit_xlib(win);
    }
    #endif

    (void)win;
    return 0;
}



void ngi_window_redisplay(ngi_window *win) {
    assert(win);
    if(!win->redisplay) {
        ngi_application* app = win->app;
        win->redisplay = 1;

        win->next_redisplay_window = app->first_redisplay_window;
        app->first_redisplay_window = win;
        
        #ifdef NGI_WINDOW_IOS
        ngi_window_redisplay_ios(win);
        #endif
        
    }

}



void ngi_window_animate(ngi_window* win, int enabled)
{
    if(enabled && !win->animating)
    {
        win->animating=1;
        win->app->num_animating++;
    }

    if(!enabled && win->animating)
    {
        win->animating=0;
        win->app->num_animating--;
    }
    
    #ifdef NGI_WINDOW_IOS
    ngi_window_animate_ios(win, enabled);
    #endif

    #ifdef NGI_WINDOW_COCOA
    ngi_window_animate_cocoa(win, enabled);
    #endif

}

