#include "ngi/ngi.h"

#ifdef NGI_WINDOW_XLIB
#include <X11/Xlib.h>
#endif

unsigned int KeySymToUcs4(KeySym keysym);


int ngi_application_init_xlib(ngi_application* app) {
    app->type = ngi_wm_api_xlib;

    XInitThreads();
    app->xlib_dpy = XOpenDisplay(NULL);

    if(app->xlib_dpy == NULL) return 0;

    return 1;
}

void ngi_application_deinit_xlib(ngi_application* app) {

    if(app->xlib_dpy) {
        XCloseDisplay(app->xlib_dpy);
    }
    
}

int ngi_application_wait_event_xlib(ngi_application* app, ngi_event* ev) {

    memset(ev,0,sizeof(ngi_event));
    
    XEvent xev;
    XNextEvent(app->xlib_dpy, &xev);
    switch(xev.type) {
        case KeyPress:
        ev->data.key.down = 1;
        ev->type = ngi_event_key_down;
        case KeyRelease:
        if(!ev->type) ev->type = ngi_event_key_up;
        {
            KeySym ks = XKeycodeToKeysym(app->xlib_dpy, xev.xkey.keycode, 0);

            if(ks != NoSymbol)
                ev->data.key.unicode = KeySymToUcs4(ks);
        }
        ev->data.key.scancode = xev.xkey.keycode;
        
        break;
    }
    
    return 1;
}

