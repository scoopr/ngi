#ifndef NGI_WINDOW_COCOA_H
#define NGI_WINDOW_COCOA_H


typedef struct {
    ngi_application *app;
    void* win;
} ngi_window_cocoa;




int ngi_window_init_cocoa(ngi_application *app, void* win);






#endif
