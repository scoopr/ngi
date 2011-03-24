#ifndef NGI_APPLICATION_H
#define NGI_APPLICATION_H


typedef struct {
    
    void *xlib_dpy;
    
} ngi_application;


void ngi_application_init(ngi_application* app);
void ngi_application_deinit(ngi_application* app);
void ngi_application_run(ngi_application* app);


void ngi_application_init_cocoa();
void ngi_application_cocoa_runloop_iteration(ngi_application* app);

void ngi_application_win32_runloop_iteration(ngi_application* app);



#endif
