
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_XLIB

#include "ngi/ngi_window.h"

#include <X11/Xlib.h>
#include <string.h>

int ngi_window_init_xlib(ngi_application *app, void* winptr) {
    
    if(app->xlib_dpy == NULL) {
        return 0;
    }

    ngi_window *win = (ngi_window*)winptr;
    win->type = ngi_window_type_xlib;

    XWindowAttributes winattr;
    XGetWindowAttributes(app->xlib_dpy, DefaultRootWindow(app->xlib_dpy), &winattr);
 
    win->platform.xlib.win = XCreateSimpleWindow(app->xlib_dpy,  // Display
                                      DefaultRootWindow(app->xlib_dpy),  // parent window
                                      0, // x
                                      0, // y
                                      winattr.width/2, // width
                                      winattr.height/2, // height
                                      0, // border width
                                      0, // border
                                      0 // background
                                      ); 
    
    Window xwin = win->platform.xlib.win;
    #if 0
    XWindowChanges xwc;
    memset(&xwc,0,sizeof(XWindowChanges));
    XConfigureWindow(app->xlib_dpy, xwin, 0, &xwc);
    #endif
    
    XSetWindowAttributes attrs;
    memset(&attrs, 0, sizeof(XSetWindowAttributes));
    attrs.event_mask = KeyPressMask;
    XChangeWindowAttributes(app->xlib_dpy, xwin, CWEventMask, &attrs);
    
    
    
    
    
    XMapWindow(app->xlib_dpy, xwin);
                                  
    return 1;
}




#endif


