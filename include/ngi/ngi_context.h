#ifndef NGI_CONTEXT_H
#define NGI_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ngi/ngi_window.h"

typedef struct ngi_context_tag {
    const char* type;
    const char* graphics;
    union {
        struct {
            void* edpy;
            void* esfc;
            void* ectx;
        } egl;

        struct {
            void* ctx;
        } cocoa;

        struct {
            void *hdc;
        } wgl;
            
    } platform;
} ngi_context;

int ngi_context_init(ngi_context* ctx, ngi_window* win);
int ngi_context_swap(ngi_context* ctx);

int ngi_context_egl_init(ngi_context* ctx, ngi_window* win);
int ngi_context_egl_swap(ngi_context* ctx);

int ngi_context_cocoa_init(ngi_context* ctx, ngi_window* win);
int ngi_context_cocoa_swap(ngi_context* ctx);

int ngi_context_wgl_init(ngi_context* ctx, ngi_window* win);
int ngi_context_wgl_swap(ngi_context* ctx);

#ifdef __cplusplus
}
#endif

#endif
