
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_COCOA

#include "ngi/ngi.h"

#import <Cocoa/Cocoa.h>


#include "ngi_window_cocoa.h"


#include <mach/mach_time.h>



double ngi_get_time() {
    static mach_timebase_info_data_t    sTimebaseInfo;
    if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
    
    uint64_t t = mach_absolute_time();
    
    return (double)t * sTimebaseInfo.numer / sTimebaseInfo.denom / 1000000000.0;
}


CVReturn displayLinkCB(
   CVDisplayLinkRef displayLink,
   const CVTimeStamp *inNow,
   const CVTimeStamp *inOutputTime,
   CVOptionFlags flagsIn,
   CVOptionFlags *flagsOut,
   void *displayLinkContext
   )
{
    (void)displayLink;
    (void)inNow;
    (void)inOutputTime;
    (void)flagsIn;
    (void)flagsOut;
    (void)flagsOut;
    
    
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    ngi_window* win = (ngi_window*)displayLinkContext;
    
    NGIWindow* window = win->plat.pwnd;
//    [window.view setNeedsDisplay:YES];

    NSArray* modes = [NSArray arrayWithObject:NSDefaultRunLoopMode];
    [[NSRunLoop mainRunLoop] performSelector:@selector(displayLinkEvent) target:window.view argument:nil order:0 modes:modes];


    [pool drain];
    
    (void)win;
    #if 0
    

    
    ngi_event ev;
    ev.type = ngi_event_redraw;
    ev.common.window = win;
//    ev.common.timestamp = inNow->videoTime / (double)inNow->videoTimeScale;
    ev.common.timestamp = ngi_get_time();

    if(win->context) ngi_context_set_active(win->context);

    ngi_post_event(win->app, &ev);
#endif    
    return kCVReturnSuccess;
}
   
   
@implementation NGIView
@synthesize win;
@synthesize oglContext;

- (id)initWithFrame:(NSRect)frame
{
    if((self = [super initWithFrame:frame]))
    {
        inputContext = [[NSTextInputContext alloc] initWithClient:self];
        inputContext.acceptsGlyphInfo = NO;
        inputContext.allowedInputSourceLocales = [NSArray arrayWithObject:NSAllRomanInputSourcesLocaleIdentifier];
        oglContext = nil;

        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);


        activeIME = NO;
        
    }
    return self;
}

-(void)startDisplayLink
{
    CVDisplayLinkStart(displayLink);
}

-(void)stopDisplayLink
{
    CVDisplayLinkStop(displayLink);
}

-(void)updateDisplayLink
{
    CGLContextObj* cglContext = [oglContext CGLContextObj];
    CGLPixelFormatObj pixelFormat = CGLGetPixelFormat(*cglContext);
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, *cglContext, pixelFormat);
//    CVDisplayLinkSetCurrentCGDisplay(displayLink, CGMainDisplayID());
    
    CVDisplayLinkSetOutputCallback(displayLink, displayLinkCB, win);
}

-(void)dealloc {
    CVDisplayLinkRelease(displayLink);
    [inputContext release];
    [super dealloc];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)becomeFirstResponder {
    return YES;
}

- (BOOL)resignFirstResponder {
    return YES;
}


- (BOOL)isOpaque
{
    return YES;
}
- (void)keyDown:(NSEvent *)theEvent {
    /*BOOL handled = */[inputContext handleEvent:theEvent];
}


- (void)mouseDown:(NSEvent *)theEvent {
    /*BOOL handled = */[inputContext handleEvent:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent {
    /*BOOL handled = */[inputContext handleEvent:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent {
    /*BOOL handled = */[inputContext handleEvent:theEvent];
}





// NSTextInputClient protocol
- (NSUInteger)characterIndexForPoint:(NSPoint)aPoint { 
    (void)aPoint;
    return NSNotFound; 
}

- (NSRect)firstRectForCharacterRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
    (void)aRange;
    (void)actualRange;
    return NSMakeRect(0,0,0,0);
}

- (NSArray *)validAttributesForMarkedText {
    return [NSArray array];
//    return [NSArray arrayWithObjects:NSMarkedClauseSegmentAttributeName, NSGlyphInfoAttributeName, nil];
}

- (NSAttributedString *)attributedSubstringForProposedRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
    (void)aRange;
    (void)actualRange;

    actualRange = NULL;

    return nil;
}
- (BOOL)hasMarkedText {

    return activeIME;
}
- (NSRange)markedRange {
    return NSMakeRange(NSNotFound, 0);
}
- (NSRange)selectedRange {
    return NSMakeRange(0,0);
}
- (void)unmarkText {

    activeIME = NO;
//    [inputContext discardMarkedText];    
}


- (void)setMarkedText:(id)aString selectedRange:(NSRange)newSelection replacementRange:(NSRange)replacementRange {
    (void)aString;
    (void)replacementRange;
    (void)newSelection;
    activeIME = YES;
//    NSLog(@"setMarkedText:%@ selectedRange:replacementRange:",aString);

}

- (void)doCommandBySelector:(SEL)aSelector {
    [super doCommandBySelector:aSelector]; // NSResponder's implementation will do nicely
}

- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange {
    (void)aString;
    (void)replacementRange;
    
//    NSLog(@"insertText:%@ replacementRange:", aString);

    [self unmarkText];
    
        
}

-(void)displayLinkEvent
{
    
    [[NSRunLoop currentRunLoop] cancelPerformSelector:@selector(displayLinkEvent) 
                                               target:self 
                                             argument:nil];
    
    [self drawRect:self.frame];
    
}

- (void)drawRect:(NSRect)dirtyRect {
    (void)dirtyRect;

    if(!win) return;
    
    ngi_window_redisplay(win);
#if 0
    ngi_event ev;
    memset(&ev,0,sizeof(ngi_event));

    ev.type = ngi_event_redraw;
    ev.common.window = win;
    ev.common.timestamp = ngi_get_time();

    if(win->context) ngi_context_set_active(win->context);

    NGIWindow *w = win->plat.pwnd;
    [w update];

//    CGLLockContext( win->context->platform.cocoa.ctx );

    ngi_post_event(win->app, &ev);
//    CGLUnlockContext( win->context->platform.cocoa.ctx );
#endif
}



@end



@implementation NGIWindow
@synthesize view;

- (id)initWithRect:(NSRect)rect
{
    NSUInteger windowStyle =  NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;
    if((self = [super initWithContentRect:rect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO]))
    {
        view = [[NGIView alloc] initWithFrame:rect];
        
        [self setContentView:view];
        [self makeFirstResponder:view];
        [self setDelegate:self];
        [self setAcceptsMouseMovedEvents:YES];
        
        
    }
    return self;
}

-(id)superview {
    return nil;
}

-(void)update {
    [view.oglContext update];
}

-(void)dealloc {
    [view release];
    
    [super dealloc];
}


- (void)windowDidResize:(NSNotification *)notification {
    (void)notification;
        
    view.win->width = view.frame.size.width;
    view.win->height = view.frame.size.height;
    
}


- (BOOL)canBecomeKeyWindow { return YES; }
- (BOOL)canBecomeMainWindow { return YES; }

- (BOOL)preservesContentDuringLiveResize {
    return NO;
}





@end







int ngi_window_init_cocoa(ngi_application *app, ngi_window* win, ngi_format* format) {
    (void)format;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    
    NSRect rect = { { 100, 100, }, { 640, 480} };
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
    
    window.view.win = win;
    
    win->width = window.view.frame.size.width;
    win->height = window.view.frame.size.height;
    
    win->redisplay = 0;
    win->next_redisplay_window = NULL;

    [pool drain];
    return 1;
}



void ngi_window_animate_cocoa(ngi_window* win, int enabled)
{
    NGIWindow* w = (NGIWindow*)win->plat.pwnd;
    NGIView* view = w.view;
    if(enabled)
    {
        [view startDisplayLink];
    } else
    {
        [view stopDisplayLink];
    }
}



int ngi_context_cocoa_init(ngi_context* ctx, ngi_window* win, ngi_format* format) {
    (void)win;
  //   NSOpenGLPixelFormatAttribute attribs[] = {
  //       NSOpenGLPFADoubleBuffer,
  //       NSOpenGLPFADepthSize, 32,
  // //      NSOpenGLPFAFullScreen,
  //       0
  //   };
    
    ctx->type = ngi_context_api_cocoa;
    ctx->graphics = ngi_graphics_api_opengl;

    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    NSOpenGLPixelFormat* pixelFormat = (NSOpenGLPixelFormat*)format->platform.cocoa.format;

    // NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];


    
//    CGLEnable(*cglContext, kCGLCEMPEngine);
  
    // GLint val = 0;
    // CGLSetParameter( *cglContext, kCGLCPMPSwapsInFlight, &val);



    NGIWindow *w = win->plat.pwnd;
    win->context = ctx;
    w.view.oglContext = context;
    [w.view updateDisplayLink];
    [context makeCurrentContext];
    [context setView:w.view];
//    [context setFullScreen];
//    int swap = 0;
//    [context setValues:&swap forParameter: NSOpenGLCPSwapInterval];
    [context update];
    ctx->platform.cocoa.ctx = context;
    [pool drain];
    
    ngi_window_redisplay(win);
    
    return 1;
}

int ngi_context_cocoa_swap(ngi_context* ctx) {
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    [(NSOpenGLContext*)ctx->platform.cocoa.ctx flushBuffer];
    [pool drain];
    return 1;
}

int ngi_context_cocoa_set_active(ngi_context* ctx) {
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    
    [(NSOpenGLContext*)ctx->platform.cocoa.ctx makeCurrentContext];

    [pool drain];
    return 1;
}


#endif
