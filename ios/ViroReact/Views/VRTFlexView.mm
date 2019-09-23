//
//  VRTFlexView.m
//  React
//
//  Created by Vik Advani on 5/9/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import <React/RCTLog.h>
#import <React/RCTBridge.h>
#import "VRTFlexView.h"
#import "VRTScene.h"
#import "VRTVideoSurface.h"
#import "VRTAnimatedComponent.h"
#import "VRTMaterialManager.h"
#import "VRTImage.h"
#import "VRTText.h"
#import "VRTVideoSurface.h"
#import "VRTNodeContainer.h"
#import "VRTQuad.h"

@implementation VRTFlexView {
    
    std::shared_ptr<VROSurface> _surface;
    std::shared_ptr<VROMaterial> _backgroundColorMaterial;
    BOOL _flexViewNeedsUpdate;
    
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if(self) {
        _width = -1.0;
        _height = -1.0;
        _flexViewNeedsUpdate = NO;
        [self node]->setHierarchicalRendering(true);
    }
    return self;
}

- (void)setHeight:(float)height {
    _height = height;
    _flexViewNeedsUpdate = YES;
}

- (void)setWidth:(float)width {
    _width = width;
    _flexViewNeedsUpdate = YES;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    // Init height if non initialized.
    if(_width < 0.0) {
        _width = 1.0;
    }
    
    if(_height < 0.0) {
        _height = 1.0;
    }
    
    [self reCalcBounds];
    if((self.materials || _backgroundColor) && _flexViewNeedsUpdate) {
        // Change the surface only if we need to.
        [self createSurface];
    }
    
    _flexViewNeedsUpdate = NO;
}

- (void)setMaterials:(NSArray<NSString *> *)materials {
    _flexViewNeedsUpdate = YES;
    [super setMaterials:materials];
}

- (void)setBackgroundColor:(UIColor *)backgroundColor {
    _backgroundColor = backgroundColor;
    _flexViewNeedsUpdate = YES;
}

- (void)reactSetFrame:(CGRect)frame {
    
    // Override and do nothing for setFrame since this is 3d spatial component that encapsulates 2d flexbox components.
    if([self isRootFlexboxView]) {
        return;
    } else {
        [super reactSetFrame:frame];
    }
}

- (BOOL)isRootFlexboxView {
    VRTView *superview = [self superview];
    if([superview isKindOfClass:[VRTAnimatedComponent class]]) {
        superview = [[super superview] superview];
    }
    
    if([superview isKindOfClass:[VRTScene class]] || [superview isKindOfClass:[VRTNodeContainer class]]) {
        return YES;
    }
    return NO;
}

- (void)sceneWillAppear {
    [self reCalcBounds];
    [super sceneWillAppear];
    
}

- (void)reCalcBounds {
    if([self isRootFlexboxView]) {
        self.bounds2DFlex = CGRectMake(0, 0, _width * k2DPointsPerSpatialUnit, _height * k2DPointsPerSpatialUnit);
        self.centerPoint2DFlex = CGPointMake( (_width * k2DPointsPerSpatialUnit/2), (_height * k2DPointsPerSpatialUnit/2));
    }
}

- (void)createSurface {
    std::shared_ptr<VROSurface> surface = VROSurface::createSurface(_width, _height);
    self.node->setGeometry(surface);
    [self applyMaterials];
    
    if (_backgroundColor) {
        CGFloat red,green,blue, alpha;
        [_backgroundColor getRed:&red green:&green blue:&blue alpha:&alpha];
        VROVector4f color(red, green, blue, alpha);
        
        surface->getMaterials().front()->getDiffuse().setColor(color);
    }
}

#pragma mark - VRTNode overrides.

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)index {
    VRTView *vrtView = (VRTView *)view;
    if ([vrtView isKindOfClass:[VRTImage class]] ||
        [vrtView isKindOfClass:[VRTText class]] ||
        [vrtView isKindOfClass:[VRTFlexView class]] ||
        [vrtView isKindOfClass:[VRTVideoSurface class]] ||
        [vrtView isKindOfClass:[VRTQuad class]]) {
        [super insertReactSubview:view atIndex:index];
    } else {
        RCTLogError(@"ERROR: Only 2D components are allowed in a Flex View container: %@", NSStringFromClass([view class]));
    }
}

@end
