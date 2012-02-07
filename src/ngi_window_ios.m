
#ifdef NGI_APPLICATION_IOS

#include "ngi/ngi.h"
#import "ngi_window_ios.h"
#import "ngi_application_ios.h"
#import <QuartzCore/QuartzCore.h>

@implementation NGIView

@synthesize win;

- (id)initWithFrame:(CGRect)frame
{
    if((self = [super initWithFrame:frame]))
    {
        UIScreen* screen = [UIScreen mainScreen];
        displayLink = [screen displayLinkWithTarget:self selector:@selector(displayLinkEvent)];
        [displayLink retain];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        displayLink.paused = YES;
        self.multipleTouchEnabled = YES;
        self.opaque = YES;
        self.userInteractionEnabled = YES;
    }
    return self;
}

-(void)dealloc
{
    [displayLink invalidate];
    [displayLink release];
    [super dealloc];
}


- (void)drawRect:(CGRect)rect
{
}


+(Class)layerClass
{
    return [CAEAGLLayer class];
}

-(void)enableDisplayLink
{
//    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    displayLink.paused = NO;
}

-(void)disableDisplayLink
{
//    [displayLink invalidate];
    displayLink.paused = YES;
}

-(void)displayLinkEvent
{
    
    ngi_event ev;
    ev.type = ngi_event_redraw;
    ev.common.window = win;
    ev.common.timestamp = displayLink.timestamp;

    if(win->context) ngi_context_set_active(win->context);

    ngi_post_event(win->app, &ev);
    
    
}

#if 0
- (BOOL)canBecomeFirstResponder
{
    return YES;
}
#endif

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [touches enumerateObjectsUsingBlock:^(id obj, BOOL* stop) {
        ngi_event ev;
        ev.type = ngi_event_touch_begin;
        ev.common.timestamp = event.timestamp;
        ev.common.window = win;
        UITouch* touch = (UITouch*)obj;
        CGPoint pos = [touch locationInView:self];
        CGPoint prev = [touch previousLocationInView:self];
        ev.touch.x = pos.x;
        ev.touch.y = pos.y;
        ev.touch.dx = pos.x - prev.x;
        ev.touch.dy = pos.x - prev.y;
        ngi_post_event(win->app, &ev);
    }];
    
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [touches enumerateObjectsUsingBlock:^(id obj, BOOL* stop) {
        ngi_event ev;
        ev.type = ngi_event_touch_end;
        ev.common.timestamp = event.timestamp;
        ev.common.window = win;
        UITouch* touch = (UITouch*)obj;
        CGPoint pos = [touch locationInView:self];
        CGPoint prev = [touch previousLocationInView:self];
        ev.touch.x = pos.x;
        ev.touch.y = pos.y;
        ev.touch.dx = pos.x - prev.x;
        ev.touch.dy = pos.x - prev.y;
        ngi_post_event(win->app, &ev);
    }];
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [touches enumerateObjectsUsingBlock:^(id obj, BOOL* stop) {
        ngi_event ev;
        ev.type = ngi_event_touch_end;
        ev.common.timestamp = event.timestamp;
        ev.common.window = win;
        UITouch* touch = (UITouch*)obj;
        CGPoint pos = [touch locationInView:self];
        CGPoint prev = [touch previousLocationInView:self];
        ev.touch.x = pos.x;
        ev.touch.y = pos.y;
        ev.touch.dx = pos.x - prev.x;
        ev.touch.dy = pos.x - prev.y;
        ngi_post_event(win->app, &ev);
    }];
    
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [touches enumerateObjectsUsingBlock:^(id obj, BOOL* stop) {
        ngi_event ev;
        ev.type = ngi_event_touch_move;
        ev.common.timestamp = event.timestamp;
        ev.common.window = win;
        UITouch* touch = (UITouch*)obj;
        CGPoint pos = [touch locationInView:self];
        CGPoint prev = [touch previousLocationInView:self];
        ev.touch.x = pos.x;
        ev.touch.y = pos.y;
        ev.touch.dx = pos.x - prev.x;
        ev.touch.dy = pos.x - prev.y;
        ngi_post_event(win->app, &ev);
    }];
    
}


@end



@implementation NGIViewController

- (id)init
{
    if((self = [super init]))
    {
        self.wantsFullScreenLayout = YES;
    }
    return self;
}


-(void)loadView
{
    UIScreen* screen = [UIScreen mainScreen];//[[UIScreen screens] objectAtIndex:1];
    self.view = [[NGIView alloc] initWithFrame:screen.bounds];
}



@end

int ngi_window_init_ios(ngi_application *app, ngi_window* win) {
//    win->plat.pwnd = [[NGIWindow alloc] initWithRect:rect];
    win->app = app;
    // win->width = window.view.frame.size.width;
    // win->height = window.view.frame.size.height;
    win->redisplay = 0;
    win->next_redisplay_window = NULL;
    
    UIWindow* window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
//    window.screen = [[UIScreen screens] objectAtIndex:1];
    window.rootViewController = [[NGIViewController alloc] init];

    NGIView* view = (NGIView*)window.rootViewController.view;

    view.win = win;
    
        
    win->plat.pwnd = window.rootViewController;

    [window makeKeyAndVisible];
    
    return 1;
}

void ngi_window_animate_ios(ngi_window* win, int enabled)
{
    NGIViewController* viewController = (NGIViewController*)win->plat.pwnd;
    
//    UIScreen* screen = viewController.view.window.screen;

    if(enabled)
    {
        [(NGIView*)viewController.view enableDisplayLink];
    }
    else
    {
        [(NGIView*)viewController.view disableDisplayLink];
    }

    
}

void ngi_window_redisplay_ios(ngi_window *win)
{
    
    if(!win->animating)
    {
        NSArray* modes = [NSArray arrayWithObject:NSDefaultRunLoopMode];
        NGIApplicationDelegate* delegate = (NGIApplicationDelegate*)[UIApplication sharedApplication].delegate;
        [[NSRunLoop currentRunLoop] performSelector:@selector(handleRedisplay) target:delegate argument:nil order:0 modes:modes];
    }
    
}



#endif


extern int dummy;
