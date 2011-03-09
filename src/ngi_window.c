
#include "ngi/ngi_window.h"



int ngi_window_create(ngi_window* win) {
    
    
    #ifdef NGI_WINDOW_XLIB
    return ngi_window_create_xlib(win);
    #endif
    
    
    return 0;
}

