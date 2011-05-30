#ifndef NGI_WINDOW_COCOA_H
#define NGI_WINDOW_COCOA_H


@interface NGIView : NSView <NSTextInputClient> {

    NSTextInputContext *inputContext;
    BOOL activeIME;
    ngi_window* win;
    
}
@property ngi_window* win;
@end


@interface NGIWindow : NSWindow <NSWindowDelegate> {
    NGIView* view;
    NSOpenGLContext* oglContext;
}

@property (assign) NGIView* view;
@property (assign) NSOpenGLContext* oglContext;

@end









#endif
