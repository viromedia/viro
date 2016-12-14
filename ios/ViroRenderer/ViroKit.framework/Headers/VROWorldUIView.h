//
//  VROUIView.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/2/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "VRORenderContext.h"
#import "VROLayer.h"
#import <memory>

/*
 UIView rendered in world space. The view is constructed as any other
 UIView, and can then be added to the 3D scene by adding its vroLayer
 to the VROScene.
 */
@interface VROWorldUIView : UIView

- (instancetype)initWithFrame:(CGRect)frame;
- (void)updateWithDriver:(VRODriver *)driver;

@property (readonly, nonatomic) std::shared_ptr<VROLayer> vroLayer;

@end
