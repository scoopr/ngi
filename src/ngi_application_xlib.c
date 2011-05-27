#include "ngi/ngi.h"

#ifdef NGI_WINDOW_XLIB
#include <X11/Xlib.h>
#endif

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
    
    
    
    
    app->first_window = NULL;

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

double get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec/10000000.0;
}

int ngi_application_wait_event_xlib(ngi_application* app, ngi_event_cb cb) {

    
    XEvent xev;
    
    ngi_event ev;
    memset(&ev,0,sizeof(ngi_event));
    

    XNextEvent(app->plat.xlib.dpy, &xev);
    double timestamp = get_timestamp();

    /*int filtered =*/ XFilterEvent(&xev, None);

    ngi_window* win = find_window(app, xev.xany.window);

    switch(xev.type) {
        case KeyPress:
        case KeyRelease:
        {

            if(!win) { printf("NULL ngi_window!\n"); return 0; }

            char buf[8];
            memset(buf,0,8);
            KeySym ks = 0;
            Status status;
            unsigned int codepoint = 0;
            if(xev.type==KeyPress /*|| xev.type==KeyRelease*/) {
                /*int ret =*/ Xutf8LookupString(win->plat.xlib.xic, &xev.xkey, buf, 7, &ks, &status);
                
                
                int utf = 0;
                int keysym = 0;
                switch(status) {
//                    case XBufferOverflow: printf("XBufferOverflow\n"); break;
                    case XLookupNone:                           break;
                    case XLookupChars:  utf = 1;                break;
                    case XLookupKeySym:          keysym = 1;    break;
                    case XLookupBoth:   utf = 1; keysym = 1;    break;
                }
                
                if( utf ) {
                    wtf8_decode(buf,7,&codepoint);
                }
            } else {
                
            }

            ks = XKeycodeToKeysym(app->plat.xlib.dpy, xev.xkey.keycode, 0);
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

            ev.type = xev.type == KeyPress ? ngi_key_down_event : ngi_key_up_event;
            ev.key.timestamp = timestamp;
            ev.key.down = xev.type == KeyPress;
            ev.key.keycode = "todo"; // TODO
            ev.key.codepoint = codepoint;
            ev.key.modifiers = 0; // TODO
            cb(&ev);

            // if(ks != NoSymbol)
            //     ev->data.key.unicode = KeySymToUcs4(ks);
        }

        
        break;
    }
    
    return 1;
}

