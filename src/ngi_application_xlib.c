#include "ngi/ngi.h"

#ifdef NGI_WINDOW_XLIB
#include <X11/Xlib.h>
#endif

#include "../../wtf8/wtf8.h"

#include <stdio.h>

unsigned int KeySymToUcs4(KeySym keysym);


int ngi_application_init_xlib(ngi_application* app) {
    memset(app, 0, sizeof(ngi_application));
    app->type = ngi_wm_api_xlib;

    XInitThreads();
    app->plat.xlib.dpy = XOpenDisplay(NULL);

    if(app->plat.xlib.dpy == NULL) return 0;


    XIM xim = XOpenIM(app->plat.xlib.dpy, NULL, NULL, NULL);
    app->plat.xlib.xim = xim;

    if(xim == NULL) {
        XCloseDisplay(app->plat.xlib.dpy);
        return 0;
    }
    
    app->plat.xlib.utf8state = 0;
    
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
        printf("%p (%p) == %p\n",(void*)cur,cur->plat.xlib.win,(void*)w);
        if(cur->plat.xlib.win == (void*)w) return cur;
        cur = cur->next_window;
    }
    
    return NULL;
    
}

int ngi_application_wait_event_xlib(ngi_application* app, ngi_event* ev) {

    memset(ev,0,sizeof(ngi_event));
    
    XEvent xev;
    
    
    XNextEvent(app->plat.xlib.dpy, &xev);

    ngi_window* win = find_window(app, xev.xany.window);

    switch(xev.type) {
        case KeyPress:
        ev->data.key.down = 1;
        ev->type = ngi_event_key_down;
        case KeyRelease:
        if(!ev->type) ev->type = ngi_event_key_up;
        {

            if(!win) { printf("NULL ngi_window!\n"); return 0; }

            char buf[8];
            memset(buf,0,8);
            KeySym ks;
            Status status;
            unsigned int codepoint = 0;
            if(xev.type==KeyPress || xev.type==KeyRelease) {
                /*int ret =*/ Xutf8LookupString(win->plat.xlib.xic, &xev.xkey, buf, 7, &ks, &status);
                unsigned char* b = (unsigned char*)buf;
                while(*b) {
                    int ret = wtf8_decode_state(&app->plat.xlib.utf8state, &codepoint, *b );
                    if(ret == UTF8_ACCEPT) {
//                        app->plat.xlib.utf8state = 0;
                        break;
                    }
                    ++b;
                }
//                wtf8_decode(buf,7,&codepoint);
            } else {
                
            }

            // KeySym ks = XKeycodeToKeysym(app->xlib_dpy, xev.xkey.keycode, 0);
            // int shifts = ShiftMask|LockMask;
            // 
            // int state = xev.xkey.state;
            // int group = 0;
            // int level = (state & shifts) ? 1 : 0;
            // KeySym ks = XkbKeycodeToKeysym ( app->plat.xlib.dpy, xev.xkey.keycode, group, level );
            // unsigned int mods = XkbKeysymToModifiers(app->plat.xlib.dpy, ks);
            ev->data.key.unicode = codepoint; //KeySymToUcs4(ks);

            // if(ks != NoSymbol)
            //     ev->data.key.unicode = KeySymToUcs4(ks);
        }
        ev->data.key.scancode = xev.xkey.keycode;
        
        break;
    }
    
    return 1;
}

