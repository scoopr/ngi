
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_COCOA

#include "ngi/ngi_application.h"
#include "ngi/ngi_window_cocoa.h"

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


int ngi_window_init_cocoa(ngi_application *app, void* winptr) {

    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    
    ngi_window_cocoa* win = (ngi_window_cocoa*)winptr;

    win->app = app;
    win->win = [[NGIWindow alloc] init];


    if(!win->win) {
        [pool drain];
        return 0;
    }


    NGIWindow* window = win->win;
    
    [window makeMainWindow];
    [window makeKeyAndOrderFront:nil];
    
    [pool drain];
    return 1;
}


void ngi_application_cocoa_runloop_iteration(ngi_application* app) {

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


#endif
