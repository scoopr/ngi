#ifndef NGI_WINDOW_XLIB_H
#define NGI_WINDOW_XLIB_H


#include "ngi/ngi_application.h"

typedef struct {
    ngi_application *app;
    unsigned long win;
} ngi_window_xlib;



int ngi_window_init_xlib(ngi_application *app, void* win);







#endif
