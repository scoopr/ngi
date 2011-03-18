
#include "ngi/ngi_config.h"
#include "ngi/ngi_application.h"

#ifdef NGI_WINDOW_XLIB
#include <X11/Xlib.h>
#endif

void ngi_application_init(ngi_application* app) {
    
    
    #ifdef NGI_WINDOW_XLIB

    XInitThreads();
    app->xlib_dpy = XOpenDisplay(NULL);
    
    #endif
    
    
    #ifdef NGI_WINDOW_COCOA
    ngi_application_init_cocoa();
    #endif
    

    (void)app;
}

void ngi_application_deinit(ngi_application* app) {
    
    
    #ifdef NGI_WINDOW_XLIB

    if(app->xlib_dpy) {
        XCloseDisplay(app->xlib_dpy);
    }
    
    #endif
    
    (void)app;
}


void ngi_application_run(ngi_application* app) {
    #ifdef NGI_WINDOW_XLIB
    XEvent ev;
    int done = 0;
    while(XNextEvent(app->xlib_dpy, &ev) && !done) {
        switch(ev.type) {
            case KeyPress:
            done=1;
            break;
        }
    }
    #endif

    #ifdef NGI_WINDOW_WIN32
        int done=0;
        while(!done) {
            ngi_application_win32_runloop_iteration(app);
        }

    #endif

    #ifdef NGI_WINDOW_COCOA
        int done=0;
        while(!done) {
            ngi_application_cocoa_runloop_iteration(app);
        }

    #endif

    (void)app;
}