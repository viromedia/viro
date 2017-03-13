//
//  VROVideoDelegate.h
//  ViroRenderer
//
//  Created by Andy Chu on 10/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoDelegate_h
#define VROVideoDelegate_h

#import <Foundation/Foundation.h>

@protocol VROVideoDelegate <NSObject>

- (void)videoDidFinish;
- (void)videoDidUpdateTime:(int)currentTimeInSeconds totalTimeInSeconds:(int)seconds;

@end

#endif /* VROVideoDelegate_h */
