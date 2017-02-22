/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */


//This class should be cut and replaced entirely from RCTShadowView when updating React
//Change the following in this source file:
//  1. Rename RCTUpdateLifeCycle to VRTUpdateLifeCycle
//  2. Rename RCTApplierBlock to VRTApplierBlock
//  3. Rename class name change from RTCShadowView to VRTShadowView
//  4. Inherit from VROComponent instead of RCTComponent
//  5. Rename instances of RCTShadowView to VRTShadowView
//  6. Rename RCTProcessMetaProps VRTProcessMetaProps
//  7. Rename all RCTProcessMetaProps in didSetProps to VRTProcessMetaProps

#import <React/RCTConvert.h>
#import <React/RCTLog.h>
#import <React/RCTUtils.h>
#import <React/UIView+React.h>
#import "VRTShadowView.h"
#import "VRTNode.h"


#import <React/UIView+Private.h>
#import "VRTAnimatedComponentShadowView.h"
#import "VRTAnimatedComponent.h"
#import "VRTImage.h"
#import "VRTFlexView.h"
#import "VRTSurface.h"
#import "VRTText.h"
#import "VRTVideoSurface.h"


typedef void (^VRTActionBlock)(VRTShadowView *shadowViewSelf, id value);
typedef void (^VRTResetActionBlock)(VRTShadowView *shadowViewSelf);

static NSString *const RCTBackgroundColorProp = @"backgroundColor";
extern const int k2DPointsPerSpatialUnit;


@implementation VRTShadowView
{
  VRTUpdateLifecycle _propagationLifecycle;
  VRTUpdateLifecycle _textLifecycle;
  NSDictionary *_lastParentProperties;
  NSMutableArray<VRTShadowView *> *_reactSubviews;
  BOOL _recomputePadding;
  BOOL _recomputeMargin;
  BOOL _recomputeBorder;
  BOOL _didUpdateSubviews;
}

@synthesize reactTag = _reactTag;


- (NSArray<VRTShadowView *> *)reactSubviews
{
  return _reactSubviews;
}

//Override padding, margin and dimension methods that convert 3d units into 2d ones for the flexbox algorithm.
// Padding
- (void)setPadding:(float)padding {
  [super setPadding:padding * k2DPointsPerSpatialUnit];
}

// Margin
- (void)setMargin:(float)margin {
  [super setMargin:margin * k2DPointsPerSpatialUnit];
}

- (void)setMarginTop:(float)marginTop {
  [super setMarginTop:marginTop * k2DPointsPerSpatialUnit];
}

- (void)setMarginBottom:(float)marginBottom {
  [super setMarginBottom:marginBottom * k2DPointsPerSpatialUnit];
}

- (void)setMarginLeft:(float)marginLeft {
  [super setMarginLeft:marginLeft * k2DPointsPerSpatialUnit];
}

- (void)setMarginRight:(float)marginRight {
  [super setMarginRight:marginRight * k2DPointsPerSpatialUnit];
}

- (void)setMarginVertical:(float)marginVertical {
  [super setMarginVertical:marginVertical * k2DPointsPerSpatialUnit];
}

- (void)setMarginHorizontal:(float)marginHorizontal {
  [super setMarginHorizontal:marginHorizontal * k2DPointsPerSpatialUnit];
}


- (void)setPaddingVertical:(float)paddingAmount{
  [super setPaddingVertical:paddingAmount * k2DPointsPerSpatialUnit];
}

- (void)setPaddingHorizontal:(float)paddingAmount {
  [super setPaddingHorizontal:paddingAmount * k2DPointsPerSpatialUnit];
}

- (void)setPaddingTop:(float)paddingAmount {
  [super setPaddingTop:paddingAmount * k2DPointsPerSpatialUnit];
}

- (void)setPaddingLeft:(float)paddingAmount {
  [super setPaddingLeft:paddingAmount * k2DPointsPerSpatialUnit];
}

- (void)setPaddingRight:(float)paddingAmount {
  [super setPaddingRight:paddingAmount * k2DPointsPerSpatialUnit];
}

- (void)setPaddingBottom:(float)paddingAmount {
  [super setPaddingBottom:paddingAmount * k2DPointsPerSpatialUnit];
}



// Border

- (void)setBorderWidth:(float)borderWidth {
  [super setBorderWidth:borderWidth * k2DPointsPerSpatialUnit];
}

- (void)setBorderTopWidth:(float)borderTopWidth {
  [super setBorderTopWidth:borderTopWidth * k2DPointsPerSpatialUnit];
}

- (void)setBorderLeftWidth:(float)borderLeftWidth {
  [super setBorderLeftWidth:borderLeftWidth * k2DPointsPerSpatialUnit];
}

- (void)setBorderRightWidth:(float)borderRightWidthAmount {
  [super setBorderRightWidth:borderRightWidthAmount * k2DPointsPerSpatialUnit];
}

- (void)setBorderBottomWidth:(float)borderBottomWidthAmount {
  [super setBorderBottomWidth:borderBottomWidthAmount * k2DPointsPerSpatialUnit];
}

// Dimensions

- (void)setWidth:(float)width {
  [super setWidth:width * k2DPointsPerSpatialUnit];
}

- (void)setHeight:(float)heightAmount {
  [super setHeight:heightAmount * k2DPointsPerSpatialUnit];
}

- (void)setMinWidth:(float)minWidthAmount {
  [super setMinWidth:minWidthAmount * k2DPointsPerSpatialUnit];
}

- (void)setMinHeight:(float)minHeightAmount {
  [super setMinHeight:minHeightAmount * k2DPointsPerSpatialUnit];
}

- (void)setMaxWidth:(float)maxWidthAmount {
  [super setMaxWidth:maxWidthAmount * k2DPointsPerSpatialUnit];
}

- (void)setMaxHeight:(float)maxHeightAmount {
  [super setMaxHeight:maxHeightAmount * k2DPointsPerSpatialUnit];
}


// Position

- (void)setTop:(float)topAmount {
  [super setTop:topAmount * k2DPointsPerSpatialUnit];
}

- (void)setBottom:(float)bottomAmount {
  [super setBottom:bottomAmount * k2DPointsPerSpatialUnit];
}

- (void)setRight:(float)rightAmount {
  [super setRight:rightAmount * k2DPointsPerSpatialUnit];
}

- (void)setLeft:(float)leftAmount {
  [super setLeft:leftAmount * k2DPointsPerSpatialUnit];
}


- (NSDictionary<NSString *, id> *)processUpdatedProperties:(NSMutableSet<RCTApplierBlock> *)applierBlocks
                                          parentProperties:(NSDictionary<NSString *, id> *)parentProperties {
  
  [applierBlocks addObject:^(NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    // If the view is not of type VRTNode then ignore.
    if(![viewRegistry[self.reactTag] isKindOfClass:[VRTNode class]]) {
      return;
    }

    VRTNode *node = (VRTNode *)viewRegistry[self.reactTag];
    VRTNode *superview = nil;

    // Root flexbox views don't need to run this block.
    if([node isRootFlexboxView]) {
      return;
    }

    // Check if this view is in a flexbox container, if not then return
    if(![self isWithinFlexBoxContainer:node]) {
      return;
    }

    
    // Find superview, skipping over animated components.
    if (node.superview && [node.superview isKindOfClass:[VRTNode class]]) {
      superview = (VRTNode *)node.superview;
    } else if(node.superview && [node.superview isKindOfClass:[VRTAnimatedComponent class]]) {
      if([[node.superview superview] isKindOfClass:[VRTNode class]]) {
        superview = (VRTNode *)[[self superview] superview];
      }
    }

    if(!superview) {
      return;
    }
    
    // Avoid crashes due to nan coords
    if (isnan(node.position2DFlex.x) || isnan(node.position2DFlex.y) ||
        isnan(node.bounds2DFlex.origin.x) || isnan(node.bounds2DFlex.origin.y) ||
        isnan(node.bounds2DFlex.size.width) || isnan(node.bounds2DFlex.size.height)) {
      RCTLogError(@"Invalid layout for (%@)%@. position: %@. bounds: %@",
                  self.reactTag, self, NSStringFromCGPoint(node.position2DFlex), NSStringFromCGRect(node.bounds2DFlex));
      return;
    }
    
    // The 2d center of the superview, ie if the parent has a width and height of 5000 points, this is: 2500,2500.
    CGPoint centerPointParent2d = [superview centerPoint2DFlex];

    // The 2d bounds, width and height of parent.
    CGRect boundsParent2d = [superview bounds2DFlex];

    if(superview != nil) {
      VROVector3f superviewPos = superview.node->getPosition();
    }

    // Flip y because our y increases as it goes 'up', instead of increasing downward with mobile.
    CGFloat transformedY = boundsParent2d.size.height - node.position2DFlex.y;

    // Transform by subtracting from center of superview.
    CGFloat transformedX = node.position2DFlex.x - centerPointParent2d.x;
    transformedY = transformedY - centerPointParent2d.y;

    // Now make into 3d bounds and 3d position
    CGFloat width3d = node.bounds2DFlex.size.width / k2DPointsPerSpatialUnit;
    CGFloat height3d =  node.bounds2DFlex.size.height / k2DPointsPerSpatialUnit;

    // Multiply by height and width of parent to get correct position
    transformedX /= k2DPointsPerSpatialUnit;
    transformedY /= k2DPointsPerSpatialUnit;

    // Always place the children of views .01 meters in front of the parent. This helps with z-fighting and ensures that the child is always in front of the parent for hit detection
    float zIncrementToAvoidZFighting = .01;
    [node node]->setPosition({(float)transformedX, (float)transformedY, zIncrementToAvoidZFighting});

    // Since VRTFlexView containers are actual size using width and height, set child components to appopriate width/height. If components don't have width/height attrib, use scale for now.
    if([node isKindOfClass:[VRTImage class]]) {
      VRTImage *image = (VRTImage *)node;
      //NSLog(@"Flex image position(%f, %f), size:(%f, %f)", transformedX, transformedY,node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit, node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit );
      [image setWidth:node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit];
      [image setHeight:node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit];
    } else if([node isKindOfClass:[VRTFlexView class]]) {
      VRTFlexView *flexview = (VRTFlexView *)node;
       //NSLog(@"Flex view position(%f, %f), size(%f, %f)", transformedX, transformedY,node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit,  node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit);
      [flexview setWidth:node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit];
      [flexview setHeight:node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit];
    }
    else if([node isKindOfClass:[VRTSurface class]]) {
      VRTSurface *surface = (VRTSurface *)node;
      //NSLog(@"Flex surface position(%f, %f), size:(%f, %f)", transformedX, transformedY,node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit, node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit );
      [surface setWidth:node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit];
      [surface setHeight:node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit];
    }
    else if([node isKindOfClass:[VRTVideoSurface class]]) {
      VRTVideoSurface *surface = (VRTVideoSurface *)node;
      //NSLog(@"Video surface position(%f, %f), size:(%f, %f)", transformedX, transformedY,node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit, node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit );
      [surface setWidth:node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit];
      [surface setHeight:node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit];
    }
    else if([node isKindOfClass:[VRTText class]]) {
      VRTText *text = (VRTText *)node;
      [text setWidth:node.bounds2DFlex.size.width/ k2DPointsPerSpatialUnit];
      [text setHeight:node.bounds2DFlex.size.height/ k2DPointsPerSpatialUnit];
    }
    else {
      //VA: TODO, VIRO-742 if we want flex for componenents that don't have width and height property then uncomment below line.
      //[self node]->setScale({(float)scale.x, (float)scale.y, 1.0});
    }
  }];
  return nil;
}

// Traverse up the view hierachy to see if the node is under a rootflexview. Return true if it is, false otherwise.
- (BOOL)isWithinFlexBoxContainer:(VRTNode *)node {
  if([node isRootFlexboxView]) {
    return YES;
  }
  
  VRTNode *superview = (VRTNode *)node.superview;
  while(superview) {
    if([superview isKindOfClass:[VRTNode class]]) {
      if([superview isRootFlexboxView]) {
        return YES;
      }
    }
    superview = superview.superview;
  }
  return NO;
}


static inline void RCTAssignSuggestedDimension(YGNodeRef cssNode, YGDimension dimension, CGFloat amount)
{
  if (amount != UIViewNoIntrinsicMetric) {
    switch (dimension) {
      case YGDimensionWidth:
        if (isnan(YGNodeStyleGetWidth(cssNode).value)) {
          YGNodeStyleSetWidth(cssNode, amount);
        }
        break;
      case YGDimensionHeight:
        if (isnan(YGNodeStyleGetHeight(cssNode).value)) {
          YGNodeStyleSetHeight(cssNode, amount);
        }
        break;
    }
  }
}

- (void)didUpdateReactSubviews
{
  // Does nothing by default
}

@end
