#ifndef NGI_CONTEXT_H
#define NGI_CONTEXT_H

#include "ngi/ngi_window.h"

typedef struct ngi_context_tag {
    union {
        struct {
            void* edpy;
            void* esfc;
        } egl;

        struct {
            void* ctx;
        } cocoa;

        struct {
            void *hdc;
        } wgl;
            
    } platform;
} ngi_context;


int ngi_context_egl_init(ngi_context* ctx, ngi_window* win);
int ngi_context_egl_swap(ngi_context* ctx);

int ngi_context_cocoa_init(ngi_context* ctx, ngi_window* win);
int ngi_context_cocoa_swap(ngi_context* ctx);

int ngi_context_wgl_init(ngi_context* ctx, ngi_window* win);
int ngi_context_wgl_swap(ngi_context* ctx);

#endif