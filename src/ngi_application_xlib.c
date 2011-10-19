#define _POSIX_C_SOURCE  199309L

#include "ngi/ngi.h"

#ifdef NGI_WINDOW_XLIB

#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>


#include "../../wtf8/wtf8.h"


#include <stdio.h>
#include <sys/time.h>

unsigned int KeySymToUcs4(KeySym keysym);




int ngi_application_init_xlib(ngi_application* app) {
    memset(app, 0, sizeof(ngi_application));
    app->type = ngi_wm_api_xlib;

    XInitThreads();
    app->plat.xlib.dpy = XOpenDisplay(NULL);

    if(app->plat.xlib.dpy == NULL) return 0;


    XSetLocaleModifiers("");

    XIM xim = XOpenIM(app->plat.xlib.dpy, NULL, NULL, NULL);
    app->plat.xlib.xim = xim;

    if(xim == NULL) {
        XCloseDisplay(app->plat.xlib.dpy);
        return 0;
    }
    
    app->plat.xlib.context = XUniqueContext();
    
//    app->first_window = NULL;

    return 1;
}

void ngi_application_deinit_xlib(ngi_application* app) {

    if(app->plat.xlib.dpy) {
        XCloseDisplay(app->plat.xlib.dpy);
    }
    if(app->plat.xlib.xim) {
        XCloseIM(app->plat.xlib.xim);
    }
    
}


#if 0
#include <stdlib.h>
ngi_window* find_window(ngi_application* app, Window w) {
    
    ngi_window* cur = app->first_window;
    
    while(cur != NULL) {
        // printf("%p (%p) == %p\n",(void*)cur,cur->plat.xlib.win,(void*)w);
        if(cur->plat.xlib.win == (void*)w) return cur;
        cur = cur->next_window;
    }
    
    return NULL;
    
}
#endif
double ngi_get_time() {
    struct timespec tv;
//    struct timeval tv;
//    gettimeofday(&tv, NULL);
    clock_gettime(CLOCK_MONOTONIC, &tv);
    return tv.tv_sec + tv.tv_nsec/1000000000.0;
//    return tv.tv_sec + tv.tv_usec/1000000.0;
}


void ngi_application_x11_handle_redisplay(ngi_application* app) {

    ngi_event ev;
    memset(&ev,0,sizeof(ngi_event));
    ngi_window* win = app->first_redisplay_window;
    app->first_redisplay_window = NULL;
    while( win != NULL) {
        
        if(win->redisplay) {
            win->redisplay = 0;
            ev.type = ngi_redraw_event;
            ev.common.window = win;
            ev.common.timestamp = ngi_get_time();

            ngi_post_event(app, &ev);
        }

        win = win->next_redisplay_window;
    }



}

void handle_X11Event(XEvent *xev, ngi_application *app) {
    double timestamp = ngi_get_time();
    ngi_event ev;
    memset(&ev,0,sizeof(ngi_event));

    /*int filtered =*/ XFilterEvent(xev, None);

    // ngi_window* win = find_window(app, xev.xany.window);
    ngi_window* win = NULL;
    XPointer winptr = NULL;
    XFindContext(app->plat.xlib.dpy, xev->xany.window, app->plat.xlib.context, &winptr);
    win = (ngi_window*)winptr;

    ngi_event_cb cb = app->event_callback;
    
    switch(xev->type) {
        case KeyPress:
        case KeyRelease:
        {

            if(!win) { printf("NULL ngi_window!\n"); return; }

            char buf[8];
            memset(buf,0,8);
            KeySym ks = 0;
            Status status;
            unsigned int codepoint = 0;
            timestamp = xev->xkey.time / 1000.0;
            if(xev->type==KeyPress /*|| xev.type==KeyRelease*/) {
                /*int ret =*/ Xutf8LookupString(win->plat.xlib.xic, &xev->xkey, buf, 7, &ks, &status);
                
                                
                int utf = 0;
                int keysym = 0;
                switch(status) {
//                    case XBufferOverflow: printf("XBufferOverflow\n"); break;
                    case XLookupNone:                           break;
                    case XLookupChars:  utf = 1;                break;
                    case XLookupKeySym:          keysym = 1;    break;
                    case XLookupBoth:   utf = 1; keysym = 1;    break;
                }
                (void)keysym;
                if( utf ) {
                    wtf8_decode(buf,7,&codepoint);
                }
            } else {
                
            }

            ks = XKeycodeToKeysym(app->plat.xlib.dpy, xev->xkey.keycode, 0);
            // int shifts = ShiftMask|LockMask;
            // 
            // int state = xev.xkey.state;
            // int group = 0;
            // int level = (state & shifts) ? 1 : 0;
            // KeySym ks = XkbKeycodeToKeysym ( app->plat.xlib.dpy, xev.xkey.keycode, group, level );
            // unsigned int mods = XkbKeysymToModifiers(app->plat.xlib.dpy, ks);

            // if(codepoint) {
            //     ev.type = ngi_character_event;
            //     ev.character.codepoint = codepoint;
            //     ev.character.timestamp = timestamp;
            //     cb(&ev);
            // }

            ev.type = xev->type == KeyPress ? ngi_key_down_event : ngi_key_up_event;
            ev.common.timestamp = timestamp;
            ev.common.window = win;
            ev.key.down = xev->type == KeyPress;
            ev.key.keycode = "todo"; // TODO
            ev.key.codepoint = codepoint;
            ev.key.modifiers = 0; // TODO
            cb(&ev);

            // if(ks != NoSymbol)
            //     ev->data.key.unicode = KeySymToUcs4(ks);
        }

        
        break;
        case Expose:

        if(xev->xexpose.count == 0) {
            ev.type = ngi_redraw_event;
            ev.common.timestamp = timestamp;
            ev.common.window = win;
            cb(&ev);


        }


        break;
        case ConfigureNotify:
        if( win->width != xev->xconfigure.width || win->height != xev->xconfigure.height) {
            ev.type = ngi_resize_event;
            ev.common.timestamp = timestamp;
            ev.common.window = win;
            win->width = ev.resize.width = xev->xconfigure.width;
            win->height = ev.resize.height = xev->xconfigure.height;
            
            cb(&ev);
        }
        
        
        
        break;
    }

}


int ngi_application_wait_event_xlib(ngi_application* app) {

    
    XEvent xev;
    Display* dpy = app->plat.xlib.dpy;

    XNextEvent(dpy, &xev);
    handle_X11Event(&xev, app);

    while(XPending(dpy) > 0) {
//    while(XEventsQueued(dpy, QueuedAfterReading) > 0) {
        XNextEvent(dpy, &xev);
        handle_X11Event(&xev, app);
    }

    ngi_application_x11_handle_redisplay(app);
    
    return 1;
}



#endif

