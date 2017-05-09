//
//  VRTPolylineManager.m
//  ViroReact
//
//  Created by Andy Chu on 5/4/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTPolylineManager.h"
#import "VRTPolyline.h"

@implementation VRTPolylineManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(points, NSArray<NSArray *>)
RCT_EXPORT_VIEW_PROPERTY(thickness, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)

- (VRTView *)view {
  return [[VRTPolyline alloc] initWithBridge:self.bridge];
}

@end
