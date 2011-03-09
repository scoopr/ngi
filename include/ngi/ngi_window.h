#ifndef NGI_WINDOW_H
#define NGI_WINDOW_H


#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_XLIB
#include "ngi/ngi_window_xlib.h"
#endif



typedef enum {
    ngi_window_type_xlib,
    ngi_window_type_cocoa,
    ngi_window_type_win32
} ngi_window_type;



typedef struct {
    ngi_window_type type;
    union {
        #ifdef NGI_WINDOW_XLIB
        ngi_window_xlib xlib;
        #endif
        
        int dummy;
    } platform;
} ngi_window;



int ngi_window_create(ngi_window* win);



#endif
