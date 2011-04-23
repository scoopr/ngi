
#include "ngi/ngi.h"

#ifdef NGI_WINDOW_XLIB
#include <X11/Xlib.h>
#endif

void ngi_application_init(ngi_application* app) {
    
    #ifdef NGI_WINDOW_COCOA
    app->type = ngi_wm_api_cocoa;
    ngi_application_init_cocoa();
    return;
    #endif

    #ifdef NGI_WINDOW_WIN32
    app->type = ngi_wm_api_win32;
    ngi_application_init_win32();
    return;
    #endif
    
    #ifdef NGI_WINDOW_XLIB
    app->type = ngi_wm_api_xlib;

    XInitThreads();
    app->xlib_dpy = XOpenDisplay(NULL);
    return;
    #endif
    
    
    

    (void)app;
}

void ngi_application_deinit(ngi_application* app) {
    
    
    #ifdef NGI_WINDOW_XLIB

    if(app->type == ngi_wm_api_xlib) {
        if(app->xlib_dpy) {
            XCloseDisplay(app->xlib_dpy);
        }
    }
    #endif
    
    (void)app;
}


void ngi_application_run(ngi_application* app) {
    #ifdef NGI_WINDOW_XLIB
    if(app->type == ngi_wm_api_xlib) {
        XEvent ev;
        int done = 0;
        while(XNextEvent(app->xlib_dpy, &ev) && !done) {
            switch(ev.type) {
                case KeyPress:
                done=1;
                break;
            }
        }
    }
    #endif

    #ifdef NGI_WINDOW_WIN32
    if(app->type == ngi_wm_api_win32) {
        int done=0;
        while(!done) {
            ngi_application_win32_runloop_iteration(app);
        }
    }
    #endif

    #ifdef NGI_WINDOW_COCOA
    if(app->type == ngi_wm_api_cocoa) {
        int done=0;
        while(!done) {
            ngi_application_cocoa_runloop_iteration(app);
        }
    }
    #endif

    (void)app;
}
