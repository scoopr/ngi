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

    struct ngi_context_tag *context;
    
    void *userdata;
    
    int width, height;
    int redisplay;
    
    int animating;

    struct ngi_window_tag* next_redisplay_window;
} ngi_window;



int ngi_window_init(ngi_application* app, ngi_window* win, ngi_format* format, ngi_config* config);
int ngi_window_deinit(ngi_window* win);


int ngi_window_init_win32(ngi_application *app, ngi_window* win, ngi_format* format);
int ngi_window_init_xlib(ngi_application *app, ngi_window* win, ngi_format* format);
int ngi_window_init_cocoa(ngi_application *app, ngi_window* win, ngi_format* format);
int ngi_window_init_ios(ngi_application *app, ngi_window* win);

int ngi_window_deinit_xlib(ngi_window* win);

void ngi_window_redisplay(ngi_window *win);

void ngi_window_animate(ngi_window* win, int enable);

void ngi_window_redisplay_ios(ngi_window *win);
void ngi_window_animate_ios(ngi_window* win, int enabled);

void ngi_window_animate_cocoa(ngi_window* win, int enabled);



#ifdef __cplusplus
}
#endif

#endif

