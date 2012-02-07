
#ifdef NGI_CONTEXT_IOS

#include "ngi/ngi.h"

#include "ngi_window_ios.h"

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <QuartzCore/CAEAGLLayer.h>


static void create_fbo(EAGLContext* context, CAEAGLLayer* layer, int* fbo_, int *rb_color, int* rb_depth)
{
    
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    GLuint colorRenderbuffer;
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER_OES, colorRenderbuffer);
    BOOL succ = [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];

    if(!succ) { fbo=0; return; }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);

    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    GLuint depthRenderbuffer;
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER_OES, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", status);
    }

    *fbo_ = fbo;
    *rb_color = colorRenderbuffer;
    *rb_depth = depthRenderbuffer;
    
}

int ngi_context_ios_init(ngi_context* ctx, ngi_window* win) 
{
    
    ctx->graphics = ngi_graphics_api_gles2;
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:nil];
    if(context == nil)
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1 sharegroup:nil];
        ctx->graphics = ngi_graphics_api_gles1;
    }
    if(context == nil)
    {
        return 0;
    }

    [EAGLContext setCurrentContext:context];


    NGIViewController* viewController = (NGIViewController*)win->plat.pwnd;

    NGIView *view = (NGIView*)viewController.view;

    CAEAGLLayer* layer = (CAEAGLLayer*)view.layer;

//    NSDictionary* properties = [NSDictionary dictionaryWithObject:kEAGLColorFormatRGBA8 forKey:kEAGLDrawablePropertyColorFormat];
//    layer.drawableProperties = properties;
    layer.opaque = YES;


    create_fbo(context, layer,
      &ctx->platform.ios.fbo,
      &ctx->platform.ios.rb_color,
      &ctx->platform.ios.rb_depth);
    

    if(ctx->platform.ios.fbo == 0) {
        NSLog(@"Main FBO creation failed");
        [context release];
        return 0;
    }


    ctx->type = ngi_context_api_ios;
    ctx->platform.ios.ctx = context;

    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    win->width = width;
    win->height = height;

    win->context = ctx;
    ngi_window_redisplay(win);

    return 1;
}



int ngi_context_ios_swap(ngi_context* ctx) 
{
    EAGLContext* context = (EAGLContext*)ctx->platform.ios.ctx;
//    [EAGLContext setCurrentContext:context];
    glBindRenderbuffer(GL_RENDERBUFFER_OES, ctx->platform.ios.rb_color);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    return 1;
}

void ngi_context_ios_set_active(ngi_context* ctx) 
{
    EAGLContext* context = (EAGLContext*)ctx->platform.ios.ctx;
    [EAGLContext setCurrentContext:context];
    glBindFramebuffer(GL_FRAMEBUFFER, ctx->platform.ios.fbo);
//    glBindRenderbuffer(GL_RENDERBUFFER_OES, ctx->platform.ios.rb_color);
}


#endif








extern int dummy;


