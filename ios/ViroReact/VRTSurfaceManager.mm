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
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onTouchViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canTouch, BOOL)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(source, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(gazeSource, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(tapSource, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)


- (VRTView *)view
{
  return [[VRTSurface alloc] initWithBridge:self.bridge];
}


@end
