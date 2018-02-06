//
//  VRTLightingEnvironmentManager.mm
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <React/RCTImageSource.h>
#import "VRTLightingEnvironmentManager.h"
#import "VRTLightingEnvironment.h"

@implementation VRTLightingEnvironmentManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(source, RCTImageSource)
RCT_EXPORT_VIEW_PROPERTY(onLoadStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLoadEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)

- (VRTLightingEnvironment *)view
{
    return [[VRTLightingEnvironment alloc] initWithBridge:self.bridge];
}

@end
