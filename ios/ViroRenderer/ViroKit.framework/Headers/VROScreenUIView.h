//
//  VROScreenUIView.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/12/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "VRORenderContext.h"
#import "VRODriver.h"
#import "VROLayer.h"
#import <memory>

class VROEye;

/*
 UIView for rendering a HUD in screen space.
 
 Deprecated: the performance of the HUD is abysmal because it requires
 re-rendering textures.
 */
@interface VROScreenUIView : UIView

- (instancetype)init;

- (void)updateWithDriver:(VRODriver *)driver;
- (void)renderEye:(VROEyeType)eye
withRenderContext:(const VRORenderContext *)renderContext
           driver:(VRODriver *)driver;

- (void)setNeedsUpdate;
- (void)setDepth:(float)depth;

@end
