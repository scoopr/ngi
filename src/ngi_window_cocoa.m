
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_COCOA

#include "ngi/ngi_window.h"
#include "ngi/ngi_context.h"

#import <Cocoa/Cocoa.h>

@interface NGIApplication : NSApplication
@end

@implementation NGIApplication
-(void)setRunning {
    /* This is the same hack used in cocoa-glut to *actually* mark the application as running */
    _running = 1;
}
@end


void ngi_application_init_cocoa() {
    

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    ProcessSerialNumber psn;
    GetCurrentProcess(&psn);
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    SetFrontProcess(&psn);

    NSApp = [[NGIApplication alloc] init];
    [NSApp finishLaunching];
    [NSApp setRunning];


    BOOL success = [NSBundle loadNibNamed:@"ngi" owner:NSApp];
    if(!success) {
    }


    [pool drain];
}


@interface NGIWindow : NSWindow
@end

@implementation NGIWindow
- (BOOL)canBecomeKeyWindow { return YES; }
- (BOOL)canBecomeMainWindow { return YES; }
@end


int ngi_window_init_cocoa(ngi_application *app, ngi_window* win) {

    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    (void)app;
    
    win->platform.pwnd = [[NGIWindow alloc] init];


    if(!win->platform.pwnd) {
        [pool drain];
        return 0;
    }


    NGIWindow* window = win->platform.pwnd;
    
    [window makeMainWindow];
    [window makeKeyAndOrderFront:nil];
    
    [pool drain];
    return 1;
}


void ngi_application_cocoa_runloop_iteration(ngi_application* app) {
    (void)app;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    int blocking=1;
    NSDate* limitDate;
    if(blocking) limitDate = [NSDate distantFuture];
    else limitDate = [NSDate distantPast];

    NSEvent* event = [NSApp nextEventMatchingMask: NSAnyEventMask
                           untilDate: limitDate
                           inMode: NSDefaultRunLoopMode
                           dequeue: YES];


    if(event.type == NSKeyDown && [event.characters length]>0 && [event.characters characterAtIndex:0] == 27) exit(1);

    [pool drain];

}


int ngi_context_cocoa_init(ngi_context* ctx, ngi_window* win) {
    (void)win;
    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        0
    };
    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];

    [context makeCurrentContext];
    ctx->platform.cocoa.ctx = context;
    [pool drain];
    return 1;
}

int ngi_context_cocoa_swap(ngi_context* ctx) {
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    [(NSOpenGLContext*)ctx->platform.cocoa.ctx flushBuffer];
    [pool drain];
    return 1;
}


#endif
