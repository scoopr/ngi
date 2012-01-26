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

    app->first_redisplay_window = NULL;

    [pool drain];
    return 1;
}



double ngi_get_time();




void handle_NSEvent(NSEvent* event, ngi_application* app) {
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
    ev.common.timestamp = event.timestamp;


    switch(event.type) {
        case NSKeyDown:
        case NSKeyUp:
        
        ev.type = event.type == NSKeyDown ? ngi_event_key_down : ngi_event_key_up;
        ev.key.down = event.type == NSKeyDown;
        ev.key.keycode = "todo"; // TODO
        ev.key.modifiers = 0; // TODO
        
        if([event.characters length]>0) {
            ev.key.codepoint = [event.characters characterAtIndex:0];
        }

        ngi_post_event(app, &ev);

        break;
        
        
        case NSLeftMouseDown      :
        case NSRightMouseDown     :
        case NSOtherMouseDown     :
        {
            ev.type = ngi_event_mouse_button;
            ev.mouse_button.button = [event buttonNumber];
            NSPoint p = [event locationInWindow];
            ev.mouse_button.x = p.x;
            ev.mouse_button.y = p.y;
            ev.mouse_button.down = 1;
            ev.mouse_button.repeats = [event clickCount];
        }

        ngi_post_event(app, &ev);
        break;
        case NSLeftMouseUp        :
        case NSRightMouseUp       :
        case NSOtherMouseUp       :
        {
            ev.type = ngi_event_mouse_button;
            ev.mouse_button.button = [event buttonNumber];
            NSPoint p = [event locationInWindow];
            ev.mouse_button.x = p.x;
            ev.mouse_button.y = p.y;
            ev.mouse_button.down = 0;
            ev.mouse_button.repeats = [event clickCount];
            ngi_post_event(app, &ev);
        }
        break;

        case NSMouseMoved         :
        {
            ev.type = ngi_event_mouse_move;
            NSPoint p = [event locationInWindow];
            ev.mouse_move.x = p.x;
            ev.mouse_move.y = p.y;
            ev.mouse_move.dx = [event deltaX];
            ev.mouse_move.dy = [event deltaY];
            ev.mouse_move.drag = 0;
            ev.mouse_move.drag_button = 0;
            ngi_post_event(app, &ev);
            
        }
        
        break;
        case NSLeftMouseDragged   :
        case NSRightMouseDragged  :
        case NSOtherMouseDragged  :
        {
            ev.type = ngi_event_mouse_move;
            NSPoint p = [event locationInWindow];
            ev.mouse_move.x = p.x;
            ev.mouse_move.y = p.y;
            ev.mouse_move.dx = [event deltaX];
            ev.mouse_move.dy = [event deltaY];
            ev.mouse_move.drag = 1;
            ev.mouse_move.drag_button = [event buttonNumber];
            ngi_post_event(app, &ev);
            
        }
        
        break;

        case NSMouseEntered       :
        case NSMouseExited        :
        break;

        case NSScrollWheel:
        ev.type = ngi_event_scroll;
        ev.scroll.dx = [event deltaX];
        ev.scroll.dy = [event deltaY];
        ev.scroll.dz = [event deltaZ];
        ngi_post_event(app, &ev);
        break;
    }
    
}


void ngi_application_cocoa_runloop_iteration(ngi_application* app, int blocking) {
    (void)app;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];


//    int blocking=1;
    NSDate* limitDate = nil;
    if(blocking) limitDate = [NSDate distantFuture];
    else limitDate = [NSDate distantPast];

    NSUInteger mask = UINT_MAX; //NSAnyEventMask;
    NSEvent* event = [NSApp nextEventMatchingMask: mask
                           untilDate: limitDate
                           inMode: NSDefaultRunLoopMode
                           dequeue: YES];


    handle_NSEvent(event, app);

    limitDate = [NSDate distantPast];
    while((event = [NSApp nextEventMatchingMask: mask
                               untilDate: limitDate
                               inMode: NSDefaultRunLoopMode
                               dequeue: YES]) != nil) {
        handle_NSEvent(event, app);
    }


//    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate distantPast]];

    // if(event.type == NSKeyDown && [event.characters length]>0 && [event.characters characterAtIndex:0] == 27) {
    //     [NSApp terminate:nil];
    // }

    ngi_application_cocoa_handle_redisplay(app);


    [pool drain];


}



#endif
