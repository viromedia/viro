//  Copyright © 2015 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This class ensures the correct values are computed and set by the RCTShadowView.
// Then the values are set by reactSetFrame on VRTNode which then invokes recalcLayout
// which then computes the view's actual size and position based on its parent's
// position and size.

#import <React/RCTConvert.h>
#import <React/RCTLog.h>
#import <React/RCTUtils.h>
#import <React/UIView+React.h>
#import "VRTShadowView.h"
#import "VRTNode.h"

#import "VRTAnimatedComponentShadowView.h"
#import "VRTAnimatedComponent.h"
#import "VRTImage.h"
#import "VRTFlexView.h"
#import "VRTQuad.h"
#import "VRTText.h"
#import "VRTVideoSurface.h"


typedef void (^VRTActionBlock)(VRTShadowView *shadowViewSelf, id value);
typedef void (^VRTResetActionBlock)(VRTShadowView *shadowViewSelf);

extern const int k2DPointsPerSpatialUnit;


@implementation VRTShadowView
{
    VRTUpdateLifecycle _propagationLifecycle;
    VRTUpdateLifecycle _textLifecycle;
    NSDictionary *_lastParentProperties;
    
    BOOL _recomputePadding;
    BOOL _recomputeMargin;
    BOOL _recomputeBorder;
    BOOL _didUpdateSubviews;
}

@synthesize reactTag = _reactTag;


//Override padding, margin and dimension methods that convert 3d units into 2d ones for the flexbox algorithm.
// Padding
- (void)setPadding:(YGValue)padding {
    if (padding.unit == YGUnitPoint) {
        padding.value *= k2DPointsPerSpatialUnit;
    }
    [super setPadding:padding];
}

// Margin
- (void)setMargin:(YGValue)margin {
    if (margin.unit == YGUnitPoint) {
        margin.value *= k2DPointsPerSpatialUnit;
    }
    [super setMargin:margin];
}

- (void)setMarginTop:(YGValue)marginTop {
    if (marginTop.unit == YGUnitPoint) {
        marginTop.value *= k2DPointsPerSpatialUnit;
    }
    [super setMarginTop:marginTop];
}

- (void)setMarginBottom:(YGValue)marginBottom {
    if (marginBottom.unit == YGUnitPoint) {
        marginBottom.value *= k2DPointsPerSpatialUnit;
    }
    [super setMarginBottom:marginBottom];
}

- (void)setMarginLeft:(YGValue)marginLeft {
    if (marginLeft.unit == YGUnitPoint) {
        marginLeft.value *= k2DPointsPerSpatialUnit;
    }
    [super setMarginLeft:marginLeft];
}

- (void)setMarginRight:(YGValue)marginRight {
    if (marginRight.unit == YGUnitPoint) {
        marginRight.value *= k2DPointsPerSpatialUnit;
    }
    [super setMarginRight:marginRight];
}

- (void)setMarginVertical:(YGValue)marginVertical {
    if (marginVertical.unit == YGUnitPoint) {
        marginVertical.value *= k2DPointsPerSpatialUnit;
    }
    [super setMarginVertical:marginVertical];
}

- (void)setMarginHorizontal:(YGValue)marginHorizontal {
    if (marginHorizontal.unit == YGUnitPoint) {
        marginHorizontal.value *= k2DPointsPerSpatialUnit;
    }
    [super setMarginHorizontal:marginHorizontal];
}

- (void)setPaddingVertical:(YGValue)paddingAmount{
    if (paddingAmount.unit == YGUnitPoint) {
        paddingAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setPaddingVertical:paddingAmount];
}

- (void)setPaddingHorizontal:(YGValue)paddingAmount {
    if (paddingAmount.unit == YGUnitPoint) {
        paddingAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setPaddingHorizontal:paddingAmount];}

- (void)setPaddingTop:(YGValue)paddingAmount {
    if (paddingAmount.unit == YGUnitPoint) {
        paddingAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setPaddingTop:paddingAmount];}

- (void)setPaddingLeft:(YGValue)paddingAmount {
    if (paddingAmount.unit == YGUnitPoint) {
        paddingAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setPaddingLeft:paddingAmount];}

- (void)setPaddingRight:(YGValue)paddingAmount {
    if (paddingAmount.unit == YGUnitPoint) {
        paddingAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setPaddingRight:paddingAmount];}

- (void)setPaddingBottom:(YGValue)paddingAmount {
    if (paddingAmount.unit == YGUnitPoint) {
        paddingAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setPaddingBottom:paddingAmount];}



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

- (void)setWidth:(YGValue)width {
    if (width.unit == YGUnitPoint) {
        width.value *= k2DPointsPerSpatialUnit;
    }
    [super setWidth:width];
}

- (void)setHeight:(YGValue)heightAmount {
    if (heightAmount.unit == YGUnitPoint) {
        heightAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setHeight:heightAmount];
}

- (void)setMinWidth:(YGValue)minWidthAmount {
    if (minWidthAmount.unit == YGUnitPoint) {
        minWidthAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setMinWidth:minWidthAmount];
}

- (void)setMinHeight:(YGValue)minHeightAmount {
    if (minHeightAmount.unit == YGUnitPoint) {
        minHeightAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setMinHeight:minHeightAmount];
}

- (void)setMaxWidth:(YGValue)maxWidthAmount {
    if (maxWidthAmount.unit == YGUnitPoint) {
        maxWidthAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setMaxWidth:maxWidthAmount];
}

- (void)setMaxHeight:(YGValue)maxHeightAmount {
    if (maxHeightAmount.unit == YGUnitPoint) {
        maxHeightAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setMaxHeight:maxHeightAmount];
}


// Position

- (void)setTop:(YGValue)topAmount {
    if (topAmount.unit == YGUnitPoint) {
        topAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setTop:topAmount];
}

- (void)setBottom:(YGValue)bottomAmount {
    if (bottomAmount.unit == YGUnitPoint) {
        bottomAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setBottom:bottomAmount];
}

- (void)setRight:(YGValue)rightAmount {
    if (rightAmount.unit == YGUnitPoint) {
        rightAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setRight:rightAmount];
}

- (void)setLeft:(YGValue)leftAmount {
    if (leftAmount.unit == YGUnitPoint) {
        leftAmount.value *= k2DPointsPerSpatialUnit;
    }
    [super setLeft:leftAmount];
}

static inline void RCTAssignSuggestedDimension(YGNodeRef cssNode, YGDimension dimension, CGFloat amount) {
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
