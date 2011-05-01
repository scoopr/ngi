
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_XLIB

#include "ngi/ngi.h"

#include <X11/Xlib.h>
#include <string.h>

int ngi_window_init_xlib(ngi_application *app, ngi_window* win) {
    
    XWindowAttributes winattr;
    Window xwin;
    XSetWindowAttributes attrs;

    if(app->xlib_dpy == NULL) {
        return 0;
    }

    win->type = ngi_window_type_xlib;

    XGetWindowAttributes(app->xlib_dpy, DefaultRootWindow(app->xlib_dpy), &winattr);
 
    win->platform.iwnd = XCreateSimpleWindow(app->xlib_dpy,
                                      DefaultRootWindow(app->xlib_dpy),
                                      0,
                                      0,
                                      winattr.width/2,
                                      winattr.height/2,
                                      0,
                                      0,
                                      0
                                      ); 
    
    xwin = win->platform.iwnd;
    #if 0
    XWindowChanges xwc;
    memset(&xwc,0,sizeof(XWindowChanges));
    XConfigureWindow(app->xlib_dpy, xwin, 0, &xwc);
    #endif
    
    memset(&attrs, 0, sizeof(XSetWindowAttributes));
    attrs.event_mask = KeyPressMask | KeyReleaseMask;
    XChangeWindowAttributes(app->xlib_dpy, xwin, CWEventMask, &attrs);
    
   
    
    
    
    XMapWindow(app->xlib_dpy, xwin);
                                  
    return 1;
}




#endif


