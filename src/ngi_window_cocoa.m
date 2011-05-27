
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_COCOA

#include "ngi/ngi.h"

#import <Cocoa/Cocoa.h>

@interface NGIApplication : NSApplication {
    int dummy;
}

@end

@implementation NGIApplication

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    (void)aNotification;
    exit(1);
}


-(void)setRunning {
    /* This is the same hack used in cocoa-glut to *actually* mark the application as running */
    _running = 1;
}
@end


int ngi_application_init_cocoa(ngi_application *app) {
    
    app->type = ngi_wm_api_cocoa;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    ProcessSerialNumber psn;
    GetCurrentProcess(&psn);
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    SetFrontProcess(&psn);


    NSApp = [NGIApplication sharedApplication];
    [NSApp setDelegate:NSApp];
    [NSApp finishLaunching];
    [NSApp setRunning];

    BOOL success = [NSBundle loadNibNamed:@"ngi" owner:NSApp];
    if(!success) {
    }



    [pool drain];
    return 1;
}


@interface NGIWindow : NSWindow {
    NSView* view;
    NSOpenGLContext* oglContext;
}
@property (assign) NSView* view;
@property (assign) NSOpenGLContext* oglContext;
@end

@implementation NGIWindow
@synthesize view;
@synthesize oglContext;

- (id)initWithRect:(NSRect)rect
{
    NSUInteger windowStyle =  NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;
    if((self = [super initWithContentRect:rect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO]))
    {
        view = [[NSView alloc] init];
        [self setContentView:view];

        oglContext = nil;
    }
    return self;
}

-(void)update {
    [oglContext update];
}

-(void)dealloc {
    [view release];
    [super dealloc];
}

- (BOOL)canBecomeKeyWindow { return YES; }
- (BOOL)canBecomeMainWindow { return YES; }
@end


int ngi_window_init_cocoa(ngi_application *app, ngi_window* win) {

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    
    NSRect rect = { { 100, 100, }, { 200, 200} };
    win->plat.pwnd = [[NGIWindow alloc] initWithRect:rect];
    win->app = app;

    if(!win->plat.pwnd) {
        [pool drain];
        return 0;
    }


    NGIWindow* window = win->plat.pwnd;
    
    [window makeMainWindow];
    [window makeKeyAndOrderFront:nil];
    [window center]; /* maybe not.. */
    
    [pool drain];
    return 1;
}


void ngi_application_cocoa_runloop_iteration(ngi_application* app, ngi_event_cb cb) {
    (void)app;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    int blocking=1;
    NSDate* limitDate = nil;
    if(blocking) limitDate = [NSDate distantFuture];
    else limitDate = [NSDate distantPast];

    NSUInteger mask = UINT_MAX; //NSAnyEventMask;
    NSEvent* event = [NSApp nextEventMatchingMask: mask
                           untilDate: limitDate
                           inMode: NSDefaultRunLoopMode
                           dequeue: YES];


//    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate distantPast]];

    // if(event.type == NSKeyDown && [event.characters length]>0 && [event.characters characterAtIndex:0] == 27) {
    //     [NSApp terminate:nil];
    // }

    ngi_event ev;
    memset(&ev,0,sizeof(ngi_event));

    [NSApp sendEvent:event];
                       
    switch(event.type) {
        case NSKeyDown:
        case NSKeyUp:
        
        ev.type = event.type == NSKeyDown ? ngi_key_down_event : ngi_key_up_event;
        ev.key.timestamp = event.timestamp;
        ev.key.down = event.type == NSKeyDown;
        ev.key.keycode = "todo"; // TODO
        ev.key.modifiers = 0; // TODO
        
        if([event.characters length]>0) {
            ev.key.codepoint = [event.characters characterAtIndex:0];
        }

        cb(&ev);

        break;
    }

    [pool drain];

}


int ngi_context_cocoa_init(ngi_context* ctx, ngi_window* win) {
    (void)win;
    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
  //      NSOpenGLPFAFullScreen,
        0
    };
    
    ctx->type = ngi_context_api_cocoa;
    ctx->graphics = ngi_graphics_api_opengl;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];

    NGIWindow *w = win->plat.pwnd;
    w.oglContext = context;
    [context makeCurrentContext];
    [context setView:w.view];
//    [context setFullScreen];
    [context update];
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
