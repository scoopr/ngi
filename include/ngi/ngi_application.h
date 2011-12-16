#ifndef NGI_APPLICATION_H
#define NGI_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

    // typedef int (*ngi_event_cb)(ngi_event* ev);

typedef struct ngi_application_tag {
    const char* type;

    ngi_event_cb event_callback;

    union {
        struct xlib_tag {
            void *dpy;
            void *xim;
            unsigned int context;
        } xlib;
        
    } plat;
    
    struct ngi_window_tag *first_redisplay_window;

    
} ngi_application;



int ngi_application_init(ngi_application* app);
int ngi_application_deinit(ngi_application* app);
//int ngi_application_run(ngi_application* app);
int ngi_application_wait_event(ngi_application* app, int blocking);


int ngi_application_init_cocoa(ngi_application* app);
void ngi_application_cocoa_runloop_iteration(ngi_application* app, int blocking);

int ngi_application_init_win32(ngi_application* app);
void ngi_application_win32_runloop_iteration(ngi_application* app, int blocking);

int ngi_application_init_xlib(ngi_application* app);
void ngi_application_deinit_xlib(ngi_application *app);
int ngi_application_wait_event_xlib(ngi_application* app, int blocking);

int ngi_post_event(ngi_application* app, ngi_event* ev);


#ifdef __cplusplus
}
#endif


#endif
