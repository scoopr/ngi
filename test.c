
#include <stdio.h>
#include <stdlib.h>

#include "ngi/ngi.h"


#ifdef NGI_WINDOW_COCOA
#include <OpenGL/gl.h>
#endif

#ifdef NGI_CONTEXT_EGL
#include <GLES2/gl2.h>
#endif

#ifdef NGI_CONTEXT_GLX
#include <GL/gl.h>
#endif

#ifdef NGI_WINDOW_WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#endif


#include <iconv.h>

char* codepointhex(int cp) {
    static char out[4*2];
    unsigned char* in = (unsigned char*)&cp;
    snprintf(out,8,"%2x%2x%2x%2x", in[0], in[1], in[2], in[3]);
    return out;
}

char* codepointutf8(int cp) {
    int cps[] = {cp};
    static char out[8];
    size_t inb = 4;
    size_t outb = 7;


    iconv_t cd = NULL;

    cd = iconv_open("UTF-8", "UCS-4LE");
    if (cd == (iconv_t) -1) {
        printf("iconv_open ERROR\n");
    }

    char* inptr = (char*)cps;
    char* outptr = out;
    memset(out,0,8);
    size_t n = iconv(cd, &inptr, &inb, &outptr, &outb);
    if( n == (size_t)-1) {
        printf("iconv ERROR\n");
    }
    return out;
}

void check_(int succ, const char* str) {
    if(succ) {
        printf("[NGI TEST] %s OK\n", str);
    } else {
        printf("[NGI TEST] %s FAIL\n", str);
        exit(1);
    }
}



#define check(x) check_( (x), #x)

int main() {

    
    ngi_application app;
    ngi_window win;
    ngi_context ctx;
    
    
    ngi_config config;
    ngi_config_init(&config);
//    ngi_config_set_string(&config, ngi_config_wm_api, ngi_wm_api_cocoa);
    

    

    printf("[NGI TEST] start\n");


    check( ngi_application_init(&app) );

    printf("[NGI TEST] wm api: %s\n", app.type);
    
    check( ngi_window_init(&app, &win, &config) );
    
//    printf("[NGI TEST] ngi_window_init: %d\n", succ);




    check( ngi_context_init(&ctx, &win) );

    printf("[NGI TEST] ctx api: %s\n", ctx.type);
    printf("[NGI TEST] gfx api: %s\n", ctx.graphics);
    
    // printf("[NGI TEST] ngi_context_*_init: %d\n", succ);
    
    glClearColor(1.0f, 0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    check( ngi_context_swap(&ctx) );


    int done = 0;
    ngi_event ev;
    while(!done) {
        ngi_application_wait_event(&app, &ev);

        if(ev.type) {
//            printf("[NGI TEST] event: %s\n", ev.type);

            if(ev.type==ngi_event_key_down || ev.type==ngi_event_key_up || ev.type==ngi_event_key_repeat)
                printf("<%s: %d \t%s\t (%s)>\n", ev.type, ev.data.key.scancode, codepointutf8(ev.data.key.unicode), codepointhex(ev.data.key.unicode));

            if(ev.type == ngi_event_key_down && ev.data.key.unicode==27) {
                done = 1;
            }
            
        }

    }


//    check( ngi_application_run(&app) );
    
    printf("[NGI TEST] end\n");
    
    check( ngi_window_deinit(&win) );
    check( ngi_application_deinit(&app) );

    return 0;
}


