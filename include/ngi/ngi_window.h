#ifndef NGI_WINDOW_H
#define NGI_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif



typedef struct ngi_window_tag {
    ngi_application* app;
    union {
        int iwnd;
        void* pwnd;
        struct {
            void* win;
            void* xic;
        } xlib;
    } plat;
    
    void *userdata;
    
    int width, height;
    
    struct ngi_window_tag* next_window;
} ngi_window;



int ngi_window_init(ngi_application* app, ngi_window* win, ngi_config* config);
int ngi_window_deinit(ngi_window* win);


int ngi_window_init_win32(ngi_application *app, ngi_window* win);
int ngi_window_init_xlib(ngi_application *app, ngi_window* win);
int ngi_window_init_cocoa(ngi_application *app, ngi_window* win);

int ngi_window_deinit_xlib(ngi_window* win);

#ifdef __cplusplus
}
#endif

#endif

