#ifndef PROBE_RENDER_GLFF_H
#define PROBE_RENDER_GLFF_H


#include "render.h"

void render_glff_init();
void render_glff_resize(int w, int h);
void render_glff_clear();
void render_glff_quad(float x, float y, float w, float h, const unsigned char color[4]);


struct render render_glff = {
    render_glff_init,
    render_glff_resize,
    render_glff_clear,
    render_glff_quad
};


#endif
