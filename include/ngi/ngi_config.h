#ifndef NGI_CONFIG_H
#define NGI_CONFIG_H


#ifndef NGI_DONT_GUESS

    #ifdef __linux
        #define NGI_WINDOW_XLIB
    #endif

    #ifdef __APPLE__
        #define NGI_WINDOW_COCOA
    #endif

    #ifdef _WIN32
        #define NGI_WINDOW_WIN32
    #endif


#endif





#endif
