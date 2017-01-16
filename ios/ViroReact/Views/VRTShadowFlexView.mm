//
//  VRTShadowFlexView.m
//  React
//
//  Created by Vik Advani on 5/17/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "RCTI18nUtil.h"
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
      CSSNodeStyleSetDirection(self.cssNode, CSSDirectionRTL);
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
      CSSNodeStyleSetWidth(self.cssNode, CSSUndefined);
      break;
    case RCTRootViewSizeFlexibilityHeight:
      CSSNodeStyleSetHeight(self.cssNode, CSSUndefined);
      break;
    case RCTRootViewSizeFlexibilityWidthAndHeight:
      CSSNodeStyleSetWidth(self.cssNode, CSSUndefined);
      CSSNodeStyleSetHeight(self.cssNode, CSSUndefined);
      break;
  }
}


@end
