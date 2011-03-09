
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_XLIB

#include "ngi/ngi_window.h"

#include <X11/Xlib.h>


int ngi_window_create_xlib(void* winptr) {
    ngi_window *win = (ngi_window*)winptr;
    win->type = ngi_window_type_xlib;
 
    Display *dpy = XOpenDisplay(NULL);
    if(dpy == NULL) return 0;
    
    
    
    return 1;
}




#endif


