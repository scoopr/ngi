
#include "ngi/ngi_window.h"



int ngi_window_init(ngi_application* app, ngi_window* win) {
    
    
    #ifdef NGI_WINDOW_XLIB
    return ngi_window_init_xlib(app, win);
    #endif
    
    
    return 0;
}

