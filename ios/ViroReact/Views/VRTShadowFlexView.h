//
//  VRTShadowViewSpatial.h
//  React
//
//  Created by vik.advani on 5/17/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTShadowView.h"


@interface VRTShadowFlexView : VRTShadowView
/**
 * Size flexibility type used to find size constraints.
 * Default to RCTRootViewSizeFlexibilityNone
 */
@property (nonatomic, assign) RCTRootViewSizeFlexibility sizeFlexibility;

/**
 * Calculate all views whose frame needs updating after layout has been calculated.
 * Returns a set contains the shadowviews that need updating.
 */
- (NSMutableOrderedSet<VRTShadowView *> *)collectViewsWithUpdatedFrames;

@end
