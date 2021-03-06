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

    int num_animating;
    int backgrounded;
    
} ngi_application;



int ngi_application_init(ngi_application* app);
int ngi_application_deinit(ngi_application* app);
//int ngi_application_run(ngi_application* app);
int ngi_application_wait_event(ngi_application* app, int blocking);


int ngi_application_init_cocoa(ngi_application* app);
void ngi_application_cocoa_runloop_iteration(ngi_application* app, int blocking);

int ngi_application_init_ios(ngi_application* app);
void ngi_application_ios_runloop_iteration(ngi_application* app, int blocking);

int ngi_application_init_win32(ngi_application* app);
void ngi_application_win32_runloop_iteration(ngi_application* app, int blocking);

int ngi_application_init_xlib(ngi_application* app);
void ngi_application_deinit_xlib(ngi_application *app);
int ngi_application_wait_event_xlib(ngi_application* app, int blocking);

int ngi_post_event(ngi_application* app, ngi_event* ev);

double ngi_get_time(void);


void ngi_application_quit(void);
void ngi_application_quit_cocoa(void);

int ngi_run(int argc, char* argv[], ngi_event_cb cb);



//internal
int ngi_application_handle_redisplay(ngi_application*);

#ifdef __cplusplus
}
#endif


#endif
