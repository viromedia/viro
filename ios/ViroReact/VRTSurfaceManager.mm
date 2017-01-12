//
//  VRTSurfaceManager.m
//  ViroReact
//
//  Created by Andy Chu on 1/11/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTSurfaceManager.h"
#import "VRTSurface.h"

@implementation VRTSurfaceManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onTapViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onGazeViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canTap, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canGaze, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)


- (VRTView *)view
{
  return [[VRTSurface alloc] initWithBridge:self.bridge];
}


@end
