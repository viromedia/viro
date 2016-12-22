//
//  VRO360PhotoManager.m
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRO360PhotoManager.h"
#import "VRT360Photo.h"
#import "RCTImageSource.h"

@implementation VRO360PhotoManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(source, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(onLoadStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLoadEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)

- (VRT360Photo *)view
{
  VRT360Photo *photo = [[VRT360Photo alloc] initWithBridge:self.bridge];
  photo.delegate = self;
  return photo;
}

@end
