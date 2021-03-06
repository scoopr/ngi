#ifndef PROBE_RENDER_GLFF_H
#define PROBE_RENDER_GLFF_H


#include "render.h"

int render_glff_is_supported();
void render_glff_init();
void render_glff_deinit();
void render_glff_resize(int w, int h);
void render_glff_clear();
void render_glff_quad(float coords[2*4], const unsigned char color[4]);
void render_glff_text(float x, float y, const char* str, ...);


struct render_t render_glff = {
    render_glff_is_supported,
    render_glff_init,
    render_glff_deinit,
    render_glff_resize,
    render_glff_clear,
    render_glff_quad,
    render_glff_text
};


#endif
