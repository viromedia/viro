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
- (void)shutdownRenderer;
- (void)renderViewDidChangeSize:(CGSize)size context:(VRORenderContext *)context;

- (void)willRenderEye:(VROEyeType)eye context:(const VRORenderContext *)context;
- (void)didRenderEye:(VROEyeType)eye context:(const VRORenderContext *)context;
- (void)reticleTapped:(VROVector3f)ray context:(const VRORenderContext *)context;
- (void)userDidRequestExitVR;

@end

#endif /* VRORenderDelegate_h */
