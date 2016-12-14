//
//  VROBoxManager.m
//  React
//
//  Created by Vik Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import "VROBoxManager.h"
#import "VRTBox.h"


@implementation VROBoxManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(length, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onTap, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onGaze, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)

- (VRTView *)view
{
  VRTBox *box = [[VRTBox alloc] initWithBridge:self.bridge];
  box.delegate = self;
  return box;
}


@end
