#include "ngi/ngi.h"


int ngi_format_init(ngi_application* app, ngi_format* format, ngi_config *config)
{
    #ifdef NGI_WINDOW_COCOA
    return ngi_format_init_cocoa(app, format, config);
    #endif
    
    #ifdef NGI_WINDOW_XLIB
    return ngi_format_init_glx(app, format, config);
    #endif
}




