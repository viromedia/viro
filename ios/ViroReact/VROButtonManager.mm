//
//  VROButtonManager.m
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VROButtonManager.h"
#import "VRTButton.h"

@implementation VROButtonManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(onTap, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onGaze, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(source, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(gazeSource, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(tapSource, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)

-(VRTButton *)view
{
  VRTButton *button = [[VRTButton alloc] initWithBridge:self.bridge];
  button.delegate = self;
  return button;
}

-(void)onTap:(VRTButton *)button {
  [super onTap:button];

  /*
   * Because Cardboard doesn't have onTap up events, we simulate the tapping event upon
   * a button by calling onTapDown then an onTapUp event. We can tie the down and up
   * events once they are available.
   */
  [button onTapDown];
  [button onTapUp];
}

-(void)onGaze:(VRTButton *)button state:(BOOL)isGazing {
  [super onGaze:button state:isGazing];
  isGazing? [button onGazeStart] : [button onGazeEnd];
}

@end