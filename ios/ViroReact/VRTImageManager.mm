//
//  VRTImageManager.mm
//  React
//
//  Created by Vik Advani on 3/4/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTImageManager.h"
#import "VRTImage.h"

@implementation VRTImageManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(onTap, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onGaze, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(onLoadStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLoadEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(source, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(placeHolderSource, UIImage)


- (VRTImage *)view
{
  VRTImage *surface = [[VRTImage alloc] initWithBridge:self.bridge];
  surface.delegate = self;
  return surface;
}

@end
