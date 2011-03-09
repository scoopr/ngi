#ifndef NGI_APPLICATION_H
#define NGI_APPLICATION_H


typedef struct {
    
    
    // xlib
    void *xlib_dpy;
    
} ngi_application;


void ngi_application_init(ngi_application* app);
void ngi_application_deinit(ngi_application* app);
void ngi_application_run(ngi_application* app);






#endif
