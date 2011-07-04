#ifndef PROBE_RENDER_OPENGL_H
#define PROBE_RENDER_OPENGL_H


#include "render.h"

void render_opengl_init();
void render_opengl_resize(int w, int h);
void render_opengl_clear();
void render_opengl_quad(float x, float y, float w, float h, const unsigned char color[4]);


struct render render_opengl = {
    render_opengl_init,
    render_opengl_resize,
    render_opengl_clear,
    render_opengl_quad
};


#endif
