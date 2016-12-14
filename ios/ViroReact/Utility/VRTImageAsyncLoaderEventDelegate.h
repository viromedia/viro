//
//  VRTAsyncLoaderEventDelegate.h
//  React
//
//  Created by Vik Advani on 9/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <UIKit/UIKit.h>

@class VRTImageAsyncLoader;
@protocol VRTImageAsyncLoaderEventDelegate

- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader;
- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image;

@end
