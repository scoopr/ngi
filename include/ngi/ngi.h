#ifndef NGI_NGI_H
#define NGI_NGI_H


#if defined(_MSC_VER) && !defined(__cplusplus)
#define inline __inline
#endif


extern const char* ngi_wm_api_egl;
extern const char* ngi_wm_api_win32;
extern const char* ngi_wm_api_xlib;
extern const char* ngi_wm_api_cocoa;
extern const char* ngi_wm_api_ios;

extern const char* ngi_context_api_egl;
extern const char* ngi_context_api_wgl;
extern const char* ngi_context_api_glx;
extern const char* ngi_context_api_cocoa;
extern const char* ngi_context_api_ios;

extern const char* ngi_graphics_api_opengl;
extern const char* ngi_graphics_api_opengl3_2;
extern const char* ngi_graphics_api_gles1;
extern const char* ngi_graphics_api_gles2;
extern const char* ngi_graphics_api_d3d9;

extern const char* ngi_pixelformat_rgb565;
extern const char* ngi_pixelformat_rgb8;
extern const char* ngi_pixelformat_rgba8;
extern const char* ngi_pixelformat_rgb16f;
extern const char* ngi_pixelformat_rgba16f;
extern const char* ngi_pixelformat_rgb32f;
extern const char* ngi_pixelformat_rgba32f;


#include "ngi/ngi_config.h"
#include "ngi/ngi_event.h"
#include "ngi/ngi_application.h"
#include "ngi/ngi_format.h"
#include "ngi/ngi_window.h"
#include "ngi/ngi_context.h"

static inline int ngi_pixelformat_total_bits(const char* pixelformat)
{
    if(ngi_config_match(ngi_pixelformat_rgb565, pixelformat)) return 16;
    if(ngi_config_match(ngi_pixelformat_rgb8, pixelformat)) return 24;
    if(ngi_config_match(ngi_pixelformat_rgba8, pixelformat)) return 32;
    if(ngi_config_match(ngi_pixelformat_rgb16f, pixelformat)) return 48;
    if(ngi_config_match(ngi_pixelformat_rgba16f, pixelformat)) return 64;
    if(ngi_config_match(ngi_pixelformat_rgb32f, pixelformat)) return 96;
    if(ngi_config_match(ngi_pixelformat_rgba32f, pixelformat)) return 128;
    return 0;
}


static inline int ngi_pixelformat_alpha_bits(const char* pixelformat)
{
    if(ngi_config_match(ngi_pixelformat_rgb565, pixelformat)) return 0;
    if(ngi_config_match(ngi_pixelformat_rgb8, pixelformat)) return 0;
    if(ngi_config_match(ngi_pixelformat_rgba8, pixelformat)) return 8;
    if(ngi_config_match(ngi_pixelformat_rgb16f, pixelformat)) return 0;
    if(ngi_config_match(ngi_pixelformat_rgba16f, pixelformat)) return 16;
    if(ngi_config_match(ngi_pixelformat_rgb32f, pixelformat)) return 0;
    if(ngi_config_match(ngi_pixelformat_rgba32f, pixelformat)) return 32;
    return 0;
}


#endif
