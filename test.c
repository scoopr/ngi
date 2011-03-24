
#include <stdio.h>

#include "ngi/ngi_window.h"


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
    
    printf("[NGI TEST] ngi_window_create: %d\n", succ);


    ngi_application_run(&app);
    
    printf("[NGI TEST] end\n");
    
    ngi_application_deinit(&app);

    return 0;
}


