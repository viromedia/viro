//
//  VRTLabelShadowView.m
//  React
//
//  Created by Vik Advani on 9/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTLabelShadowView.h"

@implementation VRTLabelShadowView


- (instancetype)init
{
  self = [super init];
  if (self) {
    //Set flex to 1 as default for text view
    [super setFlex:1];
    CSSNodeSetIsTextnode(self.cssNode, true);
  }
  return self;
}

@end
