
#ifdef NGI_CONTEXT_COCOA
#import <OpenGL/OpenGL.h>
#import <Cocoa/Cocoa.h>
#include "ngi/ngi.h"


int ngi_format_init_cocoa(ngi_application* app, ngi_format* format, ngi_config *config)
{
    (void)app;
    (void)config;
    
    const char* gfxapi = ngi_find_config_string(ngi_config_graphics_api, config, ngi_graphics_api_opengl);
    if( !ngi_config_match(gfxapi, ngi_graphics_api_opengl) 
     && !ngi_config_match(gfxapi, ngi_graphics_api_opengl3_2))
      return 0;
    
    
    int want32 = ngi_config_match(gfxapi, ngi_graphics_api_opengl3_2);
    
    const char* pixelformat = ngi_find_config_string(ngi_config_pixelformat, config, ngi_pixelformat_rgba8);
    int colorsize = ngi_pixelformat_total_bits(pixelformat);
    int alphasize = ngi_pixelformat_alpha_bits(pixelformat);
    int depthsize = ngi_find_config_int(ngi_config_depth_bits, config, 24);
    int stencilsize = ngi_find_config_int(ngi_config_stencil_bits, config, 0);
    int multisamples = ngi_find_config_int(ngi_config_multisamples, config, 0);
    
    CGLPixelFormatAttribute attribs[] = 
    {
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute) (want32 ? kCGLOGLPVersion_3_2_Core : kCGLOGLPVersion_Legacy),
        kCGLPFADoubleBuffer,
        // kCGLPFAFullScreen
        (colorsize>=48 ? kCGLPFAColorFloat : kCGLPFADoubleBuffer),
        (multisamples>0 ? kCGLPFAMultisample : kCGLPFADoubleBuffer),
        kCGLPFASampleBuffers, (CGLPixelFormatAttribute) (multisamples>0?1:0),
        kCGLPFASamples,       (CGLPixelFormatAttribute) multisamples,
        kCGLPFAColorSize,     (CGLPixelFormatAttribute) colorsize,
        kCGLPFAAlphaSize,     (CGLPixelFormatAttribute) alphasize,
        kCGLPFADepthSize,     (CGLPixelFormatAttribute) depthsize,
        kCGLPFAStencilSize,   (CGLPixelFormatAttribute) stencilsize,
        0
    };
    
    CGLPixelFormatObj cglPixelFormat;
    GLint npix;
    CGLError err = CGLChoosePixelFormat(attribs, &cglPixelFormat, &npix);
    if(err != kCGLNoError) return 0;
    if(cglPixelFormat == NULL) return 0;
    
    NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithCGLPixelFormatObj:cglPixelFormat];
    
    if(fmt == nil) return 0;
    
    
    format->platform.cocoa.format = fmt;
    
    
    return 1;
    
}
#endif
