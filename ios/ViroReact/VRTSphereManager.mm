//
//  VRTSphereManager.m
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTSphereManager.h"
#import "VRTSphere.h"

@implementation VRTSphereManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(facesOutward, BOOL)
RCT_EXPORT_VIEW_PROPERTY(widthSegmentCount, NSUInteger)
RCT_EXPORT_VIEW_PROPERTY(heightSegmentCount, NSUInteger)
RCT_EXPORT_VIEW_PROPERTY(radius, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
//RCT_EXPORT_VIEW_PROPERTY(highAccuracyGaze, BOOL)

- (VRTView *)view
{
  return [[VRTSphere alloc] initWithBridge:self.bridge];
}

@end
