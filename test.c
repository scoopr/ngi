
#include <stdio.h>

#include "ngi/ngi_window.h"

#include "ngi/ngi_context_egl.h"


#include <GLES2/gl2.h>



int main() {

    
    ngi_application app;
    ngi_window win;
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

    ngi_context_egl ctx;
    succ = ngi_context_egl_init(&ctx, &win);

    printf("[NGI TEST] ngi_context_egl_init: %d\n", succ);

    glClearColor(1.0f, 0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    ngi_context_egl_swap(&ctx);

    ngi_application_run(&app);
    
    printf("[NGI TEST] end\n");
    
    ngi_application_deinit(&app);

    return 0;
}


