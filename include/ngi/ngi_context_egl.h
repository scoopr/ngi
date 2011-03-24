#ifndef NGI_CONTEXT_EGL_H
#define NGI_CONTEXT_EGL_H

#include "ngi/ngi_window.h"

typedef struct ngi_context_egl_tag {
    void* edpy;
    void* esfc;
} ngi_context_egl;


int ngi_context_egl_init(ngi_context_egl* ctx, ngi_window* win);

int ngi_context_egl_swap(ngi_context_egl* ctx);

#endif
