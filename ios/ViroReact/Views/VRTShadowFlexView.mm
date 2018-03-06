//
//  VRTShadowFlexView.m
//  React
//
//  Created by Vik Advani on 5/17/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTI18nUtil.h>
#import "VRTShadowFlexView.h"

@implementation VRTShadowFlexView

/**
 * Init the VRTShadowFlexView with RTL status.
 * Returns a RTL CSS layout if isRTL is true (Default is LTR CSS layout).
 */
- (instancetype)init
{
    self = [super init];
    if (self) {
        if ([[RCTI18nUtil sharedInstance] isRTL]) {
            YGNodeStyleSetDirection(self.yogaNode, YGDirectionRTL);
        }
    }
    return self;
}


- (void)applySizeConstraints
{
    switch (_sizeFlexibility) {
        case RCTRootViewSizeFlexibilityNone:
            break;
        case RCTRootViewSizeFlexibilityWidth:
            YGNodeStyleSetWidth(self.yogaNode, YGUndefined);
            break;
        case RCTRootViewSizeFlexibilityHeight:
            YGNodeStyleSetHeight(self.yogaNode, YGUndefined);
            break;
        case RCTRootViewSizeFlexibilityWidthAndHeight:
            YGNodeStyleSetWidth(self.yogaNode, YGUndefined);
            YGNodeStyleSetHeight(self.yogaNode, YGUndefined);
            break;
    }
}



@end
