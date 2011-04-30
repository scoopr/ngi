#ifndef NGI_NGI_H
#define NGI_NGI_H

#include "ngi/ngi_config.h"
#include <string.h>

extern const char* ngi_wm_api_egl;
extern const char* ngi_wm_api_win32;
extern const char* ngi_wm_api_xlib;
extern const char* ngi_wm_api_cocoa;

extern const char* ngi_context_api_egl;
extern const char* ngi_context_api_wgl;
extern const char* ngi_context_api_glx;
extern const char* ngi_context_api_cocoa;

extern const char* ngi_graphics_api_opengl;
extern const char* ngi_graphics_api_opengl3_2;
extern const char* ngi_graphics_api_gles1;
extern const char* ngi_graphics_api_gles2;
extern const char* ngi_graphics_api_d3d9;

extern const char* ngi_pixelformat_rgb565;
extern const char* ngi_pixelformat_rgb8;
extern const char* ngi_pixelformat_rgba8;
extern const char* ngi_pixelformat_rgba16f;
extern const char* ngi_pixelformat_rgba32f;

extern const char* ngi_config_wm_api;     /* ngi_wm_api */
extern const char* ngi_config_context_api;     /* ngi_context_api */
extern const char* ngi_config_graphics_api;   /* ngi_graphics_api */
extern const char* ngi_config_width;                 /* int */
extern const char* ngi_config_height;               /* int */
extern const char* ngi_config_fullscreen;       /* bool/int */
extern const char* ngi_config_changemode;       /* bool/int */
extern const char* ngi_config_screen;               /* int */
extern const char* ngi_config_pixelformat;     /* ngi_pixelformat */
extern const char* ngi_config_depth_bits;       /* int */
extern const char* ngi_config_stencil_bits;   /* int */
extern const char* ngi_config_multisamples;   /* int */

extern const char* ngi_event_key_down;
extern const char* ngi_event_key_up;
extern const char* ngi_event_key_repeat;
extern const char* ngi_event_quit;



#define NGI_MAX_CONFIG 32

typedef struct ngi_config_tag {

    int num_entries;


    struct ngi_config_entry_tag {
        const char* name;
        union value_t {
            const char* s;
            int i;
        } value;
    } config[NGI_MAX_CONFIG];

} ngi_config;

static inline void ngi_config_init(ngi_config* config) {
    config->num_entries = 0;
}

static inline void ngi_config_set_int(ngi_config* config, const char* name, int value) {
    config->config[config->num_entries].name = name;
    config->config[config->num_entries].value.i = value;
    config->num_entries += 1;
}

static inline void ngi_config_set_string(ngi_config* config, const char* name, const char* value) {
    config->config[config->num_entries].name = name;
    config->config[config->num_entries].value.s = value;
    config->num_entries += 1;
}

static inline int ngi_config_match(const char* a, const char* b) {
    if( a == b ) return 1;
    return strcmp(a,b) == 0;
}


#include "ngi/ngi_application.h"
#include "ngi/ngi_window.h"
#include "ngi/ngi_context.h"


#endif
