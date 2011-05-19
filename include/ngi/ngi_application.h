#ifndef NGI_APPLICATION_H
#define NGI_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

    // typedef int (*ngi_event_cb)(ngi_event* ev);

typedef struct {
    const char* type;

    union {
        struct {
            void *dpy;
            void *xim;
        } xlib;
        
    } plat;
    
    struct ngi_window_tag *first_window;
    
} ngi_application;



int ngi_application_init(ngi_application* app);
int ngi_application_deinit(ngi_application* app);
//int ngi_application_run(ngi_application* app);
int ngi_application_wait_event(ngi_application* app, ngi_event_cb cb);


int ngi_application_init_cocoa();
void ngi_application_cocoa_runloop_iteration(ngi_application* app, ngi_event_cb cb);

void ngi_application_init_win32();
void ngi_application_win32_runloop_iteration(ngi_application* app);

int ngi_application_init_xlib(ngi_application* app);
void ngi_application_deinit_xlib(ngi_application *app);
int ngi_application_wait_event_xlib(ngi_application* app, ngi_event_cb cb);


#ifdef __cplusplus
}
#endif


#endif
