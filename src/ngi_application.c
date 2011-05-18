
#include "ngi/ngi.h"


int ngi_application_init(ngi_application* app) {


    #ifdef NGI_WINDOW_COCOA
    return ngi_application_init_cocoa();
    #endif

    #ifdef NGI_WINDOW_WIN32
    app->type = ngi_wm_api_win32;
    ngi_application_init_win32();
    return 1;
    #endif
    
    #ifdef NGI_WINDOW_XLIB
    return ngi_application_init_xlib(app);
    #endif
    
    
    (void)app;
    return 0;
}

int ngi_application_deinit(ngi_application* app) {
    
    
    #ifdef NGI_WINDOW_XLIB
    if(app->type == ngi_wm_api_xlib) {
        ngi_application_deinit_xlib(app);
    }
    #endif
    
    return 1;
    (void)app;
}

int ngi_application_wait_event(ngi_application* app, ngi_event* ev) {
    ev->type=NULL;
    #ifdef NGI_WINDOW_XLIB
    if(app->type == ngi_wm_api_xlib) {
        return ngi_application_wait_event_xlib(app, ev);
    }
    #endif

    #ifdef NGI_WINDOW_WIN32
    if(app->type == ngi_wm_api_win32) {
        ngi_application_win32_runloop_iteration(app);
    }
    #endif

    #ifdef NGI_WINDOW_COCOA
    if(app->type == ngi_wm_api_cocoa) {
        ngi_application_cocoa_runloop_iteration(app,ev);
    }
    #endif

    return 1;
    (void)app;
}

// int ngi_application_run(ngi_application* app) {
// }
