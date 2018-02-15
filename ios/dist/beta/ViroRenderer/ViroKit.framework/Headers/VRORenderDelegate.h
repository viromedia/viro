//
//  VRORenderDelegate.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRORenderDelegate_h
#define VRORenderDelegate_h

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <memory>

class VRORenderContext;
class VRODriver;
class VROVector3f;
enum class VROEyeType;

@protocol VRORenderDelegate <NSObject>

- (void)setupRendererWithDriver:(std::shared_ptr<VRODriver>)driver;
- (void)userDidRequestExitVR;

@end

#endif /* VRORenderDelegate_h */
