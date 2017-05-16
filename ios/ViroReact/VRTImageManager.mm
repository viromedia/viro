//
//  VRTImageManager.mm
//  React
//
//  Created by Vik Advani on 3/4/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTImageManager.h"
#import "VRTUIImageWrapper.h"
#import "VRTImage.h"

@implementation VRTImageManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(stereoMode, NSString)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(resizeMode, VROImageResizeMode)
RCT_EXPORT_VIEW_PROPERTY(imageClipMode, VROImageClipMode)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(onLoadStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLoadEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(source, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(placeholderSource, VRTUIImageWrapper)
RCT_EXPORT_VIEW_PROPERTY(mipmap, BOOL)
RCT_EXPORT_VIEW_PROPERTY(format, VROTextureInternalFormat)
RCT_EXPORT_VIEW_PROPERTY(physicsBody, NSDictionary)

- (VRTImage *)view
{
  return [[VRTImage alloc] initWithBridge:self.bridge];
}

@end
