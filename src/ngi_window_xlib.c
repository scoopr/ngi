
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_XLIB

#include "ngi/ngi.h"

#include <X11/Xlib.h>
#include <string.h>
#include <stdio.h>

void ngi_window_add_window_xlib(ngi_application* app, ngi_window * win) {
    
    ngi_window *cur = app->first_window;
    
    if(app->first_window == NULL) {
        app->first_window = win;
        return;
    }
    
    while(cur->next_window != NULL) {
        cur = cur->next_window;
    }
    
    cur->next_window = win;
    win->next_window = NULL;

}


int ngi_window_init_xlib(ngi_application *app, ngi_window* win) {
    
    memset(win, 0, sizeof(ngi_window));
    
    XWindowAttributes winattr;
    Window xwin;
    XSetWindowAttributes attrs;

    if(app->plat.xlib.dpy == NULL) {
        return 0;
    }

    XGetWindowAttributes(app->plat.xlib.dpy, DefaultRootWindow(app->plat.xlib.dpy), &winattr);
 
    win->app = app;
    win->plat.xlib.win = (void*)XCreateSimpleWindow((Display*)app->plat.xlib.dpy,
                                      DefaultRootWindow(app->plat.xlib.dpy),
                                      0,
                                      0,
                                      winattr.width/2,
                                      winattr.height/2,
                                      0,
                                      0,
                                      0
                                      ); 
    
    xwin = (Window)win->plat.xlib.win;
    printf("xwin: %p\n",(void*)xwin);
    #if 0
    XWindowChanges xwc;
    memset(&xwc,0,sizeof(XWindowChanges));
    XConfigureWindow(app->xlib_dpy, xwin, 0, &xwc);
    #endif
    
    memset(&attrs, 0, sizeof(XSetWindowAttributes));
    attrs.event_mask = KeyPressMask | KeyReleaseMask;
    XChangeWindowAttributes(app->plat.xlib.dpy, xwin, CWEventMask, &attrs);
    
   
    XIC xic = XCreateIC(win->app->plat.xlib.xim, 
        XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
        XNClientWindow, xwin,
        XNFocusWindow, xwin,
        NULL);
    
    win->plat.xlib.xic = xic;
        
    if(xic == NULL) {
        // TODO: XDestroyWindow etc.
        return 0;
    }
    
    XMapWindow(app->plat.xlib.dpy, xwin);
    
    ngi_window_add_window_xlib(app,win);

    return 1;
}


int ngi_window_deinit_xlib(ngi_window* win) {
    if(win->app->type != ngi_wm_api_xlib) return 0;

    if(win->plat.xlib.win) XDestroyWindow(win->app->plat.xlib.dpy, (Window)win->plat.xlib.win);
    if(win->plat.xlib.xic) XDestroyIC(win->plat.xlib.xic);
    
    
    return 1;
}



#endif


