//
//  VRTHUDComponent.m
//  React
//
//  Created by Vik Advani on 1/29/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTHUDComponent.h"
#import "VRTUtils.h"

@implementation VRTHUDLabel {
  CGFloat _screenWidth;
  CGFloat _screenHeight;
}

@synthesize position = _position;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _screenWidth = [[UIScreen mainScreen] applicationFrame].size.width;
    _screenHeight =  [[UIScreen mainScreen] applicationFrame].size.height;
    
    self.label =  [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
    [self.label setTextAlignment:NSTextAlignmentCenter];
    [self.label setFont:[UIFont systemFontOfSize:12]];
    [self.label setBackgroundColor:[UIColor clearColor]];
  }
  
  return self;
}

- (void)setText:(NSString *)text {
  [self.label setText:text];
}

- (void)setColor:(UIColor *)color {
  [self.label setTextColor:color];
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
  _position = [position copy];
  float positionValues[2];
  populateFloatArrayFromNSArray(position, positionValues, 2);
  positionValues[0] = positionValues[0] * _screenWidth;
  positionValues[1] = positionValues[1] * _screenHeight;
  [self.label setCenter:CGPointMake(positionValues[0], positionValues[1])];
}

- (void)setBackgroundColor:(UIColor *)color {
  [self.label setBackgroundColor:[UIColor clearColor]];
}

-(UIView *)getUIView {
  return self.label;
}
@end
