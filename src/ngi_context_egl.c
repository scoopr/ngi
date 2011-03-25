
#include "ngi/ngi_config.h"

#ifdef NGI_CONTEXT_EGL
#include "ngi/ngi_context.h"
#include "ngi/ngi_window.h"


#include <EGL/egl.h>
#include <stdio.h>

void checkEGL() {
    EGLint error = eglGetError();

    if(error != EGL_SUCCESS) {

        printf("EGL error %d\n", error);

    }

}


int ngi_context_egl_init(ngi_context* ctx, ngi_window* win) {

    EGLNativeDisplayType ndpy = (EGLNativeDisplayType)win->app->xlib_dpy;
    EGLNativeWindowType nwnd = (EGLNativeWindowType)win->platform.iwnd;

    EGLDisplay edpy = ctx->platform.egl.edpy = eglGetDisplay( ndpy );
    EGLint majorVersion, minorVersion;
    int succ;

    EGLConfig ecfg;
    EGLint num_config;
    EGLContext ectxt;
    EGLSurface esfc;

    EGLint ctxattr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    EGLint attr[] = {
        EGL_NONE
    };
    
    printf("eglGetDisplay: %p\n", edpy);
    checkEGL();
    succ = eglInitialize(edpy, &majorVersion, &minorVersion);
    printf("eglInitialize: %d\n", succ);
    checkEGL();

    
    if(!eglChooseConfig(edpy, attr, &ecfg, 1, &num_config)) {
        return 0;
    }
    if(num_config != 1) {
        return 0;
    }

    checkEGL();

    esfc = ctx->platform.egl.esfc = eglCreateWindowSurface(edpy, ecfg, nwnd, NULL);
    if(esfc == EGL_NO_SURFACE) {
        printf("Unable to create EGL surface (%x)\n", eglGetError());
        return 0;
    }

    ectxt = eglCreateContext(edpy, ecfg, EGL_NO_CONTEXT, ctxattr);
    if(ectxt == EGL_NO_CONTEXT) {
        printf("Unable to create EGL context (%x)\n", eglGetError());
        return 0;
    }
    eglMakeCurrent(edpy, esfc, esfc, ectxt);
    checkEGL();


    return 1;
}

int ngi_context_egl_swap(ngi_context* ctx) {
    return eglSwapBuffers(ctx->platform.egl.edpy, ctx->platform.egl.esfc);
}


#endif
