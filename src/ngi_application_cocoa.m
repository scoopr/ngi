#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_COCOA
#include "ngi/ngi.h"

#import <Cocoa/Cocoa.h>


#include "ngi_window_cocoa.h"


@interface NGIApplication : NSApplication {
    int dummy;
}

@end



@implementation NGIApplication

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    (void)aNotification;
}


-(void)setRunning {
    /* This is the same hack used in cocoa-glut to *actually* mark the application as running */
    _running = 1;
}
@end






int ngi_application_init_cocoa(ngi_application *app) {
    
    app->type = ngi_wm_api_cocoa;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    /* 
      If ran outside of application bundle, system assumes a
      command-line application or similiar. Tell system that we
      are actually a gui application that likes to have a dock icon etc.
      
    */
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

    NSWindow* nswin = event.window;
    ngi_window* win = NULL;
    if([nswin isKindOfClass:[NGIWindow class]]) {
        NGIWindow* ngiwin = (NGIWindow*)nswin;
        win = ngiwin.view.win;
    }

    ev.common.window = win;

    switch(event.type) {
        case NSKeyDown:
        case NSKeyUp:
        
        ev.type = event.type == NSKeyDown ? ngi_key_down_event : ngi_key_up_event;
        ev.common.timestamp = event.timestamp;
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



#endif
