#ifndef NGI_WINDOW_COCOA_H
#define NGI_WINDOW_COCOA_H

#import <CoreVideo/CoreVideo.h>

@interface NGIView : NSView <NSTextInputClient> {

    NSTextInputContext *inputContext;
    BOOL activeIME;
    ngi_window* win;
    CVDisplayLinkRef displayLink;
    NSOpenGLContext* oglContext;

}
@property ngi_window* win;
@property (assign) NSOpenGLContext* oglContext;

@end


@interface NGIWindow : NSWindow <NSWindowDelegate> {
    NGIView* view;
}

@property (assign) NGIView* view;

@end









#endif
