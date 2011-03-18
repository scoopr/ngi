
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

int ngi_window_init_cocoa(ngi_application *app, void* win) {
    return 0;
}


#endif
