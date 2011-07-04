#ifndef RENDER_H
#define RENDER_H

typedef void (*render_init)();
typedef void (*render_resize)(int w, int h);
typedef void (*render_clear)();
typedef void (*render_quad)(float x, float y, float w, float h, const unsigned char color[4]);


struct render {
    render_init init;
    render_resize resize;
    render_clear clear;
    render_quad quad;
};



#endif
