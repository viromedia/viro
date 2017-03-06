//
//  VROBox.m
//  React
//
//  Created by Vik Advani on 12/14/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//
#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTBox.h"
#import "VRTMaterialManager.h"


@implementation VRTBox {
  std::shared_ptr<VROBox> _box;
  bool _boxNeedsUpdate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _box = VROBox::createBox(1, 1, 1);
    _boxNeedsUpdate = NO;
    [self node]->setGeometry(_box);
  }
  
  return self;
}

- (void)setWidth:(float)width {
  if (width < 0) {
    RCTLogError(@"Box width must be >= 0");
  }
  _width = width;
  _boxNeedsUpdate = YES;
}

- (void)setHeight:(float)height {
  if (height < 0) {
    RCTLogError(@"Box height must be >= 0");
  }
  _height = height;
  _boxNeedsUpdate = YES;
}

- (void)setLength:(float)length {
  if (length < 0) {
    RCTLogError(@"Box length must be >= 0");
  }
  _length = length;
  _boxNeedsUpdate = YES;
}

- (void)updateGeometry {
  if (_boxNeedsUpdate) {
    _box = VROBox::createBox(_width, _height, _length);
    [self node]->setGeometry(_box);
    
    _boxNeedsUpdate = NO;
  }
  [self applyMaterials];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  [self updateGeometry];
}

@end


