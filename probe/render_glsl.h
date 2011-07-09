#ifndef PROBE_RENDER_GLSL_H
#define PROBE_RENDER_GLSL_H


#include "render.h"

void render_glsl_init();
void render_glsl_resize(int w, int h);
void render_glsl_clear();
void render_glsl_quad(float x, float y, float w, float h, const unsigned char color[4]);


struct render render_glsl = {
    render_glsl_init,
    render_glsl_resize,
    render_glsl_clear,
    render_glsl_quad
};

#endif
