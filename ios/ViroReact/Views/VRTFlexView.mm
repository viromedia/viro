//
//  VRTFlexView.m
//  React
//
//  Created by Vik Advani on 5/9/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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
#import "VRTSurface.h"

@implementation VRTFlexView {
  bool _surfaceNeedsUpdate;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if(self) {
    _width = -1.0;
    _height = -1.0;
    _surfaceNeedsUpdate = NO;
      
    [self node]->setHierarchicalRendering(true);
  }
  return self;
}

- (void)setHeight:(float)height {
  _height = height;
  _surfaceNeedsUpdate = YES;
  [self reCalcBounds];
}

- (void)setWidth:(float)width {
  _width = width;
  _surfaceNeedsUpdate = YES;
  [self reCalcBounds];
}

- (void)setFlexBackgroundColor:(UIColor *)backgroundColor {
  _flexBackgroundColor = backgroundColor;
  _surfaceNeedsUpdate = YES;
}

- (void)reactSetFrame:(CGRect)frame {
  
  //override and do nothing for setFrame since this is 3d spatial component that encapsulates 2d flexbox components.
  if([self isRootFlexboxView]) {
    return;
  } else {
    [super reactSetFrame:frame];
  }
}

- (BOOL)isRootFlexboxView {
  if([[self superview] isKindOfClass:[VRTScene class]] || [[self superview] isKindOfClass:[VRTNodeContainer class]]) {
    return YES;
  }
  return NO;
}

- (void)reCalcBounds {
  self.bounds2DFlex = CGRectMake(0, 0, _width * k2DPointsPerSpatialUnit, _height * k2DPointsPerSpatialUnit);
  self.centerPoint2DFlex = CGPointMake( (_width * k2DPointsPerSpatialUnit/2), (_height * k2DPointsPerSpatialUnit/2));
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  if (_surfaceNeedsUpdate) {
    if ((self.materials || self.backgroundColor) && _width > 0.0) {
      [self createSurface];
    }
  }
  _surfaceNeedsUpdate = NO;
}

- (void)createSurface {
  //set defaults to 1.0 if not set.
  if(_width < 0.0) {
    _width = 1.0;
  }
  
  if(_height < 0.0) {
    _height = 1.0;
  }
  
  std::shared_ptr<VROSurface> surface = VROSurface::createSurface(_width, _height);
  self.node->setGeometry(surface);
  [self applyMaterials];
  
  if (_flexBackgroundColor) {
    CGFloat red,green,blue, alpha;
    [_flexBackgroundColor getRed:&red green:&green blue:&blue alpha:&alpha];
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
          [vrtView isKindOfClass:[VRTAnimatedComponent class]] ||
          [vrtView isKindOfClass:[VRTSurface class]]) {
    [super insertReactSubview:view atIndex:index];
  } else {
    RCTLogError(@"ERROR: Only 2D components are allowed in a Flex View container: %@", NSStringFromClass([view class]));
  }
}

@end
