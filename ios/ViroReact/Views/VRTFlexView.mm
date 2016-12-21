//
//  VRTFlexView.m
//  React
//
//  Created by Vik Advani on 5/9/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTFlexView.h"
#import "VRTScene.h"
#import "VRTVideoSurface.h"
#import "VRTAnimatedComponent.h"
#import "VROMaterialManager.h"
#import "RCTLog.h"
#import "RCTBridge.h"
#import "VRTImage.h"
#import "VRTButton.h"
#import "VRTLabel.h"
#import "VRTVideoSurface.h"
#import "VRTNodeContainer.h"

@implementation VRTFlexView {
  std::shared_ptr<VROSurface> _surface;
  std::shared_ptr<VROMaterial> _backgroundColorMaterial;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if(self) {
    _surface = NULL;
    _width = -1.0;
    _height = -1.0;
  }
  return self;
}

- (void)setHeight:(float)height {
  _height = height;
  [self reCalcBounds];
  if(_surface && (self.materials || self.backgroundColor) && _width > 0.0) {
    [self createSurface];
  }
}

- (void)setWidth:(float)width {
  _width = width;
  [self reCalcBounds];
  if(_surface && (self.materials || self.backgroundColor) && _height > 0.0) {
    [self createSurface];
  }
}

- (void)setBackgroundColor:(UIColor *)backgroundColor {
  _backgroundColor = backgroundColor;
  if(_backgroundColor != nil) {
    std::shared_ptr<VROMaterial> newMaterial =  std::make_shared<VROMaterial>();
    newMaterial->setWritesToDepthBuffer(true);
    newMaterial->setReadsFromDepthBuffer(true);
    // Create new background color material.
    CGFloat red,green,blue, alpha;
    [_backgroundColor getRed:&red green:&green blue:&blue alpha:&alpha];
    VROVector4f vecColor(red, green, blue, alpha);
    newMaterial->getDiffuse().setColor(vecColor);
    _backgroundColorMaterial = newMaterial;
    if(_surface && _height > 0.0 && _width > 0.0) {
      [self createSurface];
    }
  }
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

- (void)viewWillAppear {
  if(self.materials != nil || self.backgroundColor!=nil) {
    [self createSurface];
  }
  [super viewWillAppear];

}

- (void)reCalcBounds {
  self.bounds2D = CGRectMake(0, 0, _width * k2DPointsPerSpatialUnit, _height * k2DPointsPerSpatialUnit);
  self.centerPoint2D = CGPointMake( (_width * k2DPointsPerSpatialUnit/2), (_height * k2DPointsPerSpatialUnit/2));
}

- (void)createSurface {
  //set defaults to 1.0 if not set.
  if(_width < 0.0) {
    _width = 1.0;
  }
  
  if(_height < 0.0) {
    _height = 1.0;
  }
  
  // Background color takes precedence over a material set by the user.
  if(self.materials != nil || _backgroundColor != nil) {
    _surface = VROSurface::createSurface(_width, _height);
    [self node]->setGeometry(_surface);
    if(_backgroundColorMaterial) {
      [self node]->getGeometry()->getMaterials().clear();
      [self node]->getGeometry()->getMaterials().push_back(_backgroundColorMaterial);
      return;
    }

    if(self.materials) {
      [self applyMaterials];
      return;
    }
  } else {
    [self node]->setGeometry(NULL);
  }
}

#pragma mark - VRTNode overrides.

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)index {
  VRTView *vrtView = (VRTView *)view;
  if ([vrtView isKindOfClass:[VRTImage class]] ||
          [vrtView isKindOfClass:[VRTButton class]] ||
          [vrtView isKindOfClass:[VRTLabel class]] ||
          [vrtView isKindOfClass:[VRTFlexView class]] ||
          [vrtView isKindOfClass:[VRTVideoSurface class]] ||
          [vrtView isKindOfClass:[VRTAnimatedComponent class]]) {
    [super insertReactSubview:view atIndex:index];
  } else {
    RCTLogError(@"ERROR: Only 2D components are allowed in a Flex View container: %@", NSStringFromClass([view class]));
  }
}

@end
