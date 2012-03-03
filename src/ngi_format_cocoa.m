
#ifdef NGI_CONTEXT_COCOA
#import <Cocoa/Cocoa.h>
#include "ngi/ngi.h"

int ngi_format_init_cocoa(ngi_application* app, ngi_format* format, ngi_config *config)
{
    (void)app;
    (void)config;
    
    NSOpenGLPixelFormatAttribute attribs[] = 
    {
        NSOpenGLPFADoubleBuffer,
        // NSOpenGLPFAFullScreen
        // NSOpenGLPFAColorFloat,
        // NSOpenGLPFAMultisample,
        // NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1
        // NSOpenGLPFASamples,       (NSOpenGLPixelFormatAttribute)4,
        NSOpenGLPFAColorSize,     (NSOpenGLPixelFormatAttribute)32,
        // NSOpenGLPFAAlphaSize,     (NSOpenGLPixelFormatAttribute)0,
        NSOpenGLPFADepthSize,     (NSOpenGLPixelFormatAttribute)16,
        // NSOpenGLPFAStencilSize,   (NSOpenGLPixelFormatAttribute)8,
        0
    };
    NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    
    if(fmt == nil) return 0;
    
    
    format->platform.cocoa.format = fmt;
    
    
    return 1;
    
}
#endif
