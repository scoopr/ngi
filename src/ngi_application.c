
#include "ngi/ngi.h"

static int gApplicationQuit;

void ngi_application_quit()
{
    gApplicationQuit = 1;
    
    #ifdef NGI_WINDOW_COCOA
    ngi_application_quit_cocoa();
    #endif
    
}

int ngi_application_init(ngi_application* app) {


    #ifdef NGI_WINDOW_COCOA
    return ngi_application_init_cocoa(app);
    #endif

    #ifdef NGI_WINDOW_WIN32
    return ngi_application_init_win32(app);
    #endif
    
    #ifdef NGI_WINDOW_XLIB
    return ngi_application_init_xlib(app);
    #endif

    #ifdef NGI_WINDOW_IOS
    return ngi_application_init_ios(app);
    #endif
    
    
    (void)app;
    return 0;
}

int ngi_application_deinit(ngi_application* app) {
    
    
    #ifdef NGI_WINDOW_XLIB
    if(app->type == ngi_wm_api_xlib) {
        ngi_application_deinit_xlib(app);
    }
    #endif
    
    return 1;
    (void)app;
}

int ngi_application_wait_event(ngi_application* app, int blocking) {

    #ifdef NGI_WINDOW_XLIB
    if(app->type == ngi_wm_api_xlib) {
        return ngi_application_wait_event_xlib(app, blocking);
    }
    #endif

    #ifdef NGI_WINDOW_WIN32
    if(app->type == ngi_wm_api_win32) {
        ngi_application_win32_runloop_iteration(app, blocking);
    }
    #endif

    #ifdef NGI_WINDOW_COCOA
    if(app->type == ngi_wm_api_cocoa) {
        ngi_application_cocoa_runloop_iteration(app, blocking);
    }
    #endif

    #ifdef NGI_WINDOW_IOS
    if(app->type == ngi_wm_api_ios) {
        ngi_application_ios_runloop_iteration(app, blocking);
    }
    #endif

    return 1;
    (void)app;
}


int ngi_post_event(ngi_application* app, ngi_event* ev) {
    return app->event_callback(ev);
}


int ngi_application_handle_redisplay(ngi_application* app) {

    ngi_event ev;
    ngi_window* first = app->first_redisplay_window;
    ngi_window* win = first;
    
    if(app->backgrounded) return 0;
    
    memset(&ev,0,sizeof(ngi_event));

    
    app->first_redisplay_window = NULL;
    while( win != NULL) {
  
  
        ngi_window* nextwin = win->next_redisplay_window;
        if(win->redisplay) {
            if(!win->animating) {
                win->redisplay = 0;
            }
            win->next_redisplay_window = NULL;
            ev.type = ngi_event_redraw;
            ev.common.window = win;
            ev.common.timestamp = ngi_get_time();

            if(win->context) ngi_context_set_active(win->context);

            ngi_post_event(app, &ev);
            
            if(win->animating) {
                win->next_redisplay_window = app->first_redisplay_window;
                app->first_redisplay_window = win;

            }
        }

        win = nextwin;
    }
    

    
    return !!app->first_redisplay_window;

}


#ifndef NGI_APPLICATION_IOS
int ngi_run(int argc, char** argv, ngi_event_cb cb)
{
    ngi_application app;
    ngi_event ev;
    ngi_application_init(&app);
    
    app.event_callback = cb;
    
    ev.type = ngi_event_application_init;
    ev.common.timestamp = ngi_get_time();
    ev.common.window = NULL;
    ev.application_init.application = &app;
    
    gApplicationQuit = 0;
    ngi_post_event(&app, &ev);
    
    while(!gApplicationQuit)
    {
        ngi_application_wait_event(&app, app.num_animating == 0 || app.backgrounded);
    }


    ngi_application_deinit(&app);
	return 0;
    (void)argc;
    (void)argv;
}
#endif

// int ngi_application_run(ngi_application* app) {
// }
