#ifndef NGI_CONFIG_H
#define NGI_CONFIG_H


#include <string.h>

extern const char* ngi_config_wm_api;     /* ngi_wm_api */
extern const char* ngi_config_context_api;     /* ngi_context_api */
extern const char* ngi_config_graphics_api;   /* ngi_graphics_api */
extern const char* ngi_config_width;                 /* int */
extern const char* ngi_config_height;               /* int */
// extern const char* ngi_config_fullscreen;       /* bool/int */
// extern const char* ngi_config_changemode;       /* bool/int */
// extern const char* ngi_config_screen;               /* int */
extern const char* ngi_config_pixelformat;     /* ngi_pixelformat */
extern const char* ngi_config_depth_bits;       /* int */
extern const char* ngi_config_stencil_bits;   /* int */
extern const char* ngi_config_multisamples;   /* int */

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





static inline const char* ngi_find_config_string(const char* name, ngi_config* config, const char* fallback) {

    int i;
    
    if(config == 0) return fallback;

    for(i = 0; i < config->num_entries; ++i)
    {
        if(ngi_config_match(config->config[i].name, name)) return config->config[i].value.s;
    }

    return fallback;

}

static inline int ngi_find_config_int(const char* name, ngi_config* config, int fallback) {

    int i;
    
    if(config == 0) return fallback;

    for(i = 0; i < config->num_entries; ++i)
    {
        if(ngi_config_match(config->config[i].name, name)) return config->config[i].value.i;
    }

    return fallback;

}


#endif
