#ifndef NGI_CONFIG_H
#define NGI_CONFIG_H


#ifndef NGI_DONT_GUESS

    #ifdef __linux
        #define NGI_WINDOW_XLIB
        #define NGI_CONTEXT_EGL
        #define NGI_CONTEXT_GLX
    #endif

    #ifdef __APPLE__
        #define NGI_WINDOW_COCOA
        #define NGI_CONTEXT_COCOA
    #endif

    #ifdef _WIN32
        #define NGI_WINDOW_WIN32
        #define NGI_CONTEXT_WGL
    #endif
    

#endif





#endif
