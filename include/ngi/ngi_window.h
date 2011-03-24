#ifndef NGI_WINDOW_H
#define NGI_WINDOW_H


#include "ngi/ngi_config.h"
#include "ngi/ngi_application.h"


typedef enum {
    ngi_window_type_xlib,
    ngi_window_type_cocoa,
    ngi_window_type_win32
} ngi_window_type;



typedef struct ngi_window_tag {
    ngi_window_type type;
    ngi_application* app;
    union {
        int iwnd;
        void* pwnd;
    } platform;
} ngi_window;



int ngi_window_init(ngi_application* app, ngi_window* win);


int ngi_window_init_win32(ngi_application *app, ngi_window* win);
int ngi_window_init_xlib(ngi_application *app, ngi_window* win);
int ngi_window_init_cocoa(ngi_application *app, ngi_window* win);



#endif

