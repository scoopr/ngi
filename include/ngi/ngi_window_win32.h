#ifndef NGI_WINDOW_WIN32_H
#define NGI_WINDOW_WIN32_H




#include "ngi/ngi_application.h"


#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <windows.h>


typedef struct {
    ngi_application *app;
    HWND hwnd;
} ngi_window_win32;



int ngi_window_init_win32(ngi_application *app, void* win);









#endif
