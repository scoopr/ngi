#ifndef RENDER_H
#define RENDER_H

typedef int (*render_is_supported)();
typedef void (*render_init)();
typedef void (*render_resize)(int w, int h);
typedef void (*render_clear)();
typedef void (*render_quad)(float x, float y, float w, float h, const unsigned char color[4]);
typedef void (*render_text)(float x, float y, const char* str, ...);


struct render_t {
    render_is_supported is_supported;
    render_init init;
    render_resize resize;
    render_clear clear;
    render_quad quad;
    render_text text;
};



#endif
