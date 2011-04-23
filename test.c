
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
//#include <GL/gl.h>
#endif

#ifdef NGI_WINDOW_WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#endif


void check_(int succ, const char* str) {
    if(succ) {
        printf("[ngi] %s OK\n", str);
    } else {
        printf("[ngi] %s FAIL\n", str);
        exit(1);
    }
}

#define check(x) check_( (x), #x)

int main() {

    
    ngi_application app;
    ngi_window win;
    ngi_context ctx;
    int succ;
    
    
    ngi_config config;
    ngi_config_init(&config);
//    ngi_config_set_string(&config, ngi_config_wm_api, ngi_wm_api_cocoa);
    

    

    printf("[NGI TEST] start\n");


    #ifdef NGI_WINDOW_XLIB
    printf("[NGI TEST] Support NGI_WINDOW_XLIB\n");
    #endif

    #ifdef NGI_WINDOW_COCOA
    printf("[NGI TEST] Support NGI_WINDOW_COCOA\n");
    #endif

    #ifdef NGI_WINDOW_WIN32
    printf("[NGI TEST] Support NGI_WINDOW_WIN32\n");
    #endif



    ngi_application_init(&app);

    printf("Using wm api: %s\n", app.type);
    
    check( ngi_window_init(&app, &win, &config) );
    
//    printf("[NGI TEST] ngi_window_init: %d\n", succ);



    #ifdef NGI_CONTEXT_EGL
    if(app.type == ngi_wm_api_xlib)
        check( ngi_context_egl_init(&ctx, &win) );
    #endif

    #ifdef NGI_CONTEXT_COCOA
    if(app.type == ngi_wm_api_cocoa)
        check( ngi_context_cocoa_init(&ctx, &win) );
    #endif
    #ifdef NGI_CONTEXT_WGL
    check( ngi_context_wgl_init(&ctx, &win) );
    #endif
    
    // printf("[NGI TEST] ngi_context_*_init: %d\n", succ);
    
    glClearColor(1.0f, 0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    #ifdef NGI_CONTEXT_EGL
    ngi_context_egl_swap(&ctx);
    #endif
    #ifdef NGI_CONTEXT_COCOA
    ngi_context_cocoa_swap(&ctx);
    #endif
	#ifdef NGI_CONTEXT_WGL
	ngi_context_wgl_swap(&ctx);
	#endif

    ngi_application_run(&app);
    
    printf("[NGI TEST] end\n");
    
    ngi_application_deinit(&app);

    return 0;
}


