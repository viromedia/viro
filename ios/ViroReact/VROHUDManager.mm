//
//  VROHUDManager.m
//  React
//
//  Created by Vik Advani on 1/29/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VROHUDManager.h"
#import "VRTHud.h"
#import "VRTHUDComponent.h"

@implementation VROHUDManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(reticle, BOOL)

- (VRTView *)view {
    return [[VRTHUD alloc] init];
}

@end

@implementation VROHUDLabelManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(text, NSString)
RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray);
//RCT_EXPORT_VIEW_PROPERTY(backgroundColor, UIColor);

- (VRTView *)view {
    return [[VRTHUDLabel alloc] initWithBridge:self.bridge];
}

@end

