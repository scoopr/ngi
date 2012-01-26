
#ifdef NGI_APPLICATION_IOS

#include "ngi/ngi.h"
#import "ngi_window_ios.h"
#import <QuartzCore/QuartzCore.h>

@implementation NGIView
- (id)initWithFrame:(CGRect)frame
{
    if((self = [super initWithFrame:frame]))
    {
        NSLog(@"NGIView -initWithFrame:(%fx%f)",frame.size.width, frame.size.height);
        
    }
    return self;
}

-(void)dealloc
{
    [super dealloc];
}


- (void)drawRect:(CGRect)rect
{
    NSLog(@"NGIView -drawRect:");
}


+(Class)layerClass
{
    return [CAEAGLLayer class];
}
@end



@implementation NGIViewController

- (id)init
{
    if((self = [super init]))
    {
        NSLog(@"NGIViewController -init");
        self.wantsFullScreenLayout = YES;
        
    }
    return self;
}


-(void)loadView
{
    NSLog(@"NGIViewController -loadView");
    
    UIScreen* screen = [UIScreen mainScreen];//[[UIScreen screens] objectAtIndex:1];
    self.view = [[NGIView alloc] initWithFrame:screen.bounds];
}



@end

int ngi_window_init_ios(ngi_application *app, ngi_window* win) {
//    win->plat.pwnd = [[NGIWindow alloc] initWithRect:rect];
    NSLog(@"ngi_window_init_ios");
    win->app = app;
    // win->width = window.view.frame.size.width;
    // win->height = window.view.frame.size.height;
    win->redisplay = 0;
    win->next_redisplay_window = NULL;
    
    UIWindow* window = [[UIWindow alloc] init];
//    window.screen = [[UIScreen screens] objectAtIndex:1];
    window.rootViewController = [[NGIViewController alloc] init];
        
    win->plat.pwnd = window.rootViewController;

    [window makeKeyAndVisible];
    
    return 1;
}




#endif


extern int dummy;
