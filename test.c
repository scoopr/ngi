
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


    check( ngi_application_run(&app) );
    
    printf("[NGI TEST] end\n");
    
    check( ngi_application_deinit(&app) );

    return 0;
}


