
#ifdef NGI_APPLICATION_IOS

#import <UIKit/UIKit.h>

#include <mach/mach_time.h>


#include "ngi/ngi.h"
#import "ngi_window_ios.h"
#import "ngi_application_ios.h"


int ngi_application_init_ios(ngi_application* app)
{
    memset(app, 0, sizeof(ngi_application));
    app->type = ngi_wm_api_ios;
    app->first_redisplay_window = NULL;
    app->backgrounded = 0;
    return 1;
}

void ngi_application_ios_runloop_iteration(ngi_application* app, int blocking)
{
    NSDate* date = [NSDate distantPast];
    if(blocking) date = [NSDate distantFuture];
    [[NSRunLoop currentRunLoop] runUntilDate:date];
    ngi_application_handle_redisplay(app);
    
}



double ngi_get_time() {
    static mach_timebase_info_data_t    sTimebaseInfo;
    if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
    
    uint64_t t = mach_absolute_time();
    
    return (double)t * sTimebaseInfo.numer / sTimebaseInfo.denom / 1000000000.0;
}


ngi_event_cb gEventCallback = NULL;

@implementation NGIApplicationDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    [[NSFileManager defaultManager] changeCurrentDirectoryPath:bundlePath];

    ngi_application_init(&app);
    app.event_callback = gEventCallback;
    
    ngi_event ev;
    ev.type = ngi_event_application_init;
    ev.common.window = NULL;
    ev.common.timestamp = ngi_get_time();
    ev.application_init.application = &app;
    app.event_callback(&ev);
    
    
//    ngi_application_handle_redisplay(&app);
    
    NSArray* modes = [NSArray arrayWithObject:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] performSelector:@selector(run) target:self argument:nil order:0 modes:modes];
    
    return YES;
}

-(void)run
{
    

    /*
    while(true)
    {
        ngi_application_wait_event(&app, app.num_animating == 0 || app.backgrounded);
    }
*/

    
    
}

-(void)handleRedisplay
{
    ngi_application_handle_redisplay(&app);
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    app.backgrounded = 1;
}
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    app.backgrounded = 0;
}

@end





int ngi_run(int argc, char* argv[], ngi_event_cb cb)
{
    gEventCallback = cb;
    return UIApplicationMain(argc, argv, nil, @"NGIApplicationDelegate");
}





#endif

extern int dummy;
