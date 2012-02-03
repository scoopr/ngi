#ifndef NGI_WINDOW_IOS_H
#define NGI_WINDOW_IOS_H

#import <UIKit/UIKit.h>

#include "ngi/ngi.h"

@interface NGIView : UIView
{
    CADisplayLink* displayLink;
    ngi_window* win;
}

@property ngi_window* win;

- (id)initWithFrame:(CGRect)aRect;

-(void)enableDisplayLink;
-(void)disableDisplayLink;


@end

@interface NGIViewController : UIViewController
{
}
@end



#endif
