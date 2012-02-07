#ifndef NGI_APPLICATION_IOS_H
#define NGI_APPLICATION_IOS_H

#import <UIKit/UIKit.h>

#include "ngi/ngi.h"


@interface NGIApplicationDelegate : NSObject <UIApplicationDelegate>
{
    NGIViewController* viewController;
    ngi_application app;
}

-(void)handleRedisplay;
@end

#endif
