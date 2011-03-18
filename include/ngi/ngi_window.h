#ifndef NGI_WINDOW_H
#define NGI_WINDOW_H


#include "ngi/ngi_config.h"
#include "ngi/ngi_application.h"

#ifdef NGI_WINDOW_XLIB
#include "ngi/ngi_window_xlib.h"
#endif

#ifdef NGI_WINDOW_WIN32
#include "ngi/ngi_window_win32.h"
#endif

#ifdef NGI_WINDOW_COCOA
#include "ngi/ngi_window_cocoa.h"
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
        #ifdef NGI_WINDOW_WIN32
        ngi_window_win32 win32;
        #endif
        
        int dummy;
    } platform;
} ngi_window;



int ngi_window_init(ngi_application* app, ngi_window* win);



#endif
