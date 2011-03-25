
#include <stdio.h>

#include "ngi/ngi_window.h"

#include "ngi/ngi_context.h"

#ifdef NGI_WINDOW_COCOA
#include <OpenGL/gl.h>
#endif

#ifdef NGI_CONTEXT_EGL
#include <GLES2/gl2.h>
#endif

#ifdef NGI_WINDOW_WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/GL.h>
#endif

int main() {

    
    ngi_application app;
    ngi_window win;
    ngi_context ctx;
    int succ;

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
    
    succ = ngi_window_init(&app, &win);
    
    printf("[NGI TEST] ngi_window_init: %d\n", succ);



    #ifdef NGI_CONTEXT_EGL
    succ = ngi_context_egl_init(&ctx, &win);
    #endif

    #ifdef NGI_CONTEXT_COCOA
    succ = ngi_context_cocoa_init(&ctx, &win);
    #endif
	#ifdef NGI_CONTEXT_WGL
	succ = ngi_context_wgl_init(&ctx, &win);
	#endif
    
    printf("[NGI TEST] ngi_context_*_init: %d\n", succ);
    
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


