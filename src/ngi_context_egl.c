
#include "ngi/ngi_config.h"

#ifdef NGI_CONTEXT_EGL
#include "ngi/ngi.h"

#include <EGL/egl.h>
#include <stdio.h>

void checkEGL() {
    EGLint error = eglGetError();

    if(error != EGL_SUCCESS) {

        printf("EGL error %d\n", error);

    }

}


int ngi_context_egl_init(ngi_context* ctx, ngi_window* win) {

    EGLNativeDisplayType ndpy = NULL;
    
    EGLNativeWindowType nwnd = 0;
    
    #ifdef NGI_WINDOW_XLIB
    if(win->app->type == ngi_wm_api_xlib ) {
        ndpy = (EGLNativeDisplayType)win->app->plat.xlib.dpy;
        nwnd = (EGLNativeWindowType)win->plat.xlib.win;
    }
    #endif

    ctx->type = ngi_context_api_egl;
    EGLDisplay edpy = ctx->platform.egl.edpy = eglGetDisplay( ndpy );
    EGLint majorVersion, minorVersion;
    int succ;

    EGLConfig ecfg;
    EGLint num_config;
    EGLContext ectxt;
    EGLSurface esfc;

    EGLint ctxattr[] = { 
        EGL_CONTEXT_CLIENT_VERSION, 2, 
        
        EGL_NONE 
        
        };

    EGLint attr[] = {
 /*       EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT | EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT,*/
/*        EGL_NATIVE_RENDERABLE, 1,*/
/*        EGL_CONFIG_CAVEAT, EGL_NONE,*/
        EGL_NONE
    };
    
    
    printf("eglGetDisplay: %p\n", edpy);
    checkEGL();
    succ = eglInitialize(edpy, &majorVersion, &minorVersion);
    printf("eglInitialize: %d\n", succ);
    checkEGL();

//    eglBindAPI(EGL_OPENGL_API);

    const char* apis = eglQueryString(edpy, EGL_CLIENT_APIS);
    const char* exts = eglQueryString(edpy, EGL_EXTENSIONS);
        
    printf("EGL APIs: %s\n", apis);
    printf("EGL Extensions: %s\n", exts);

    
    if(!eglChooseConfig(edpy, attr, &ecfg, 1, &num_config)) {
        printf("eglChooseConfig error\n");
        return 0;
    }
    if(num_config != 1) {
        printf("More than one config: %d\n", num_config);
        return 0;
    }

    checkEGL();

    esfc = ctx->platform.egl.esfc = eglCreateWindowSurface(edpy, ecfg, nwnd, NULL);
    if(esfc == EGL_NO_SURFACE) {
        printf("Unable to create EGL surface (%x)\n", eglGetError());
        return 0;
    }
    ectxt = ctx->platform.egl.ectx = eglCreateContext(edpy, ecfg, EGL_NO_CONTEXT, ctxattr);
    if(ectxt == EGL_NO_CONTEXT) {
        printf("Unable to create EGL context (%x)\n", eglGetError());
        return 0;
    }
    eglMakeCurrent(edpy, esfc, esfc, ectxt);
    
    checkEGL();

    EGLint val = 0;
    eglQueryContext(edpy, ectxt, EGL_CONTEXT_CLIENT_TYPE, &val);
    printf("Context client type: %x\n", val);
    eglQueryContext(edpy, ectxt, EGL_CONTEXT_CLIENT_VERSION, &val);
    printf("Context client version: %d\n", val);

    EGLenum api = eglQueryAPI();
    int ver=0;

    switch(api) {
        case EGL_OPENGL_ES_API:
        eglQueryContext(edpy, ectxt, EGL_CONTEXT_CLIENT_VERSION, &ver);
        switch(ver) {
            case 1: ctx->graphics = ngi_graphics_api_gles1; break;
            case 2: ctx->graphics = ngi_graphics_api_gles2; break;
            default: printf("Unsupported GLES version %d\n", ver);
        }
        break;
        case EGL_OPENGL_API:
        ctx->graphics = ngi_graphics_api_opengl;
        break;
        default:
        printf("Unsupported EGL Graphics api 0x%x\n", api);
    }

    eglSwapInterval(edpy,1);

    win->context = ctx;

    return 1;
}

int ngi_context_egl_swap(ngi_context* ctx) {
    return eglSwapBuffers(ctx->platform.egl.edpy, ctx->platform.egl.esfc);
}


#endif

extern int dummy;
