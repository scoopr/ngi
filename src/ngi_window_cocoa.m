
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



@implementation NGIView
@synthesize win;

- (id)initWithFrame:(NSRect)frame
{
    if((self = [super initWithFrame:frame]))
    {
        inputContext = [[NSTextInputContext alloc] initWithClient:self];
        inputContext.acceptsGlyphInfo = NO;
        inputContext.allowedInputSourceLocales = [NSArray arrayWithObject:NSAllRomanInputSourcesLocaleIdentifier];

        activeIME = NO;
        
    }
    return self;
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

-(void)dealloc {
    [inputContext release];
    [super dealloc];
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

- (void)drawRect:(NSRect)dirtyRect {
    (void)dirtyRect;

    if(!win) return;

    ngi_event ev;
    memset(&ev,0,sizeof(ngi_event));

    ev.type = ngi_event_redraw;
    ev.common.window = win;
    ev.common.timestamp = ngi_get_time();

    NGIWindow *w = win->plat.pwnd;
    [w update];

    ngi_post_event(win->app, &ev);
}



@end



@implementation NGIWindow
@synthesize view;
@synthesize oglContext;

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
        
        oglContext = nil;
    }
    return self;
}

-(id)superview {
    return nil;
}

-(void)update {
    [oglContext update];
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







int ngi_window_init_cocoa(ngi_application *app, ngi_window* win) {

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
    win->context = ctx;
    w.oglContext = context;
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
