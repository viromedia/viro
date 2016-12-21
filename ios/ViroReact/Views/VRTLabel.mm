//
//  VRTLabel.m
//  React
//
//  Created by Vik Advani on 1/12/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
#import <CoreText/CoreText.h>
#import "VRTLabel.h"
#import "VROUtils.h"
#import "ViroUIManager.h"
#import "VRTFlexView.h"
#import "RCTConvert.h"
#import "RCTLog.h"
#import "RCTFont.h"

NSString *const VRTLabelReactTagAttributeName = @"ReactTagAttributeName";

/*
 * This number reflects how much text in iOS 2D coordinates
 * we want to fit into 1 spatial unit. If the number is too large, the text can appear to small regardless of 'font size'.
 * If too small the opposite can happen. The current number seems to be a good medium, where text is readable at distances
 * of 1 unit or more at various font sizes.
 *
 */
static const CGFloat kTextPointsPerSpatialCoord = 500;
static const CGFloat kTextMaxVerticalLength = 2000;

@implementation VRTLabel {
  NSString *_text;
  CGFloat _cachedTextStorageWidth;
  float _ratio;
  NSMutableAttributedString *_cachedMutableAttributedString;
  CGFloat _effectiveLetterSpacing;
  BOOL _boundsCalculated;
  BOOL _needsUpdating;
  CGRect _frame;
  
}

@synthesize position = _position;
@synthesize rotation = _rotation;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    //TODO self.vroUIView = [[VROWorldUIView alloc] initWithFrame:CGRectMake(0, 0, 100, 10)];
    //TODO self.vroUIView.vroLayer->getMaterial()->setWritesToDepthBuffer(true);
    //TODO self.vroUIView.vroLayer->getMaterial()->setReadsFromDepthBuffer(true);
    _boundsCalculated = false;
    _width = 1.0;
    _ratio = 1.0;
    _needsUpdating = NO;
  }
  
  return self;
}

- (void)setText:(NSString *)text {
  _text = text;
  // TODO if (self.label && self.driver) {
    BOOL isWithinFlexView =  [self isWithinFlexView];
    [self sizeAndUpdateLabel:_frame isInFlexView:isWithinFlexView];
  // TODO  [self.vroUIView updateWithDriver:self.driver];
  // TODO }
}

- (NSAttributedString *)_attributedStringWithFontFamily:(NSString *)fontFamily
                                               fontSize:(NSNumber *)fontSize
                                             fontWeight:(NSString *)fontWeight
                                              fontStyle:(NSString *)fontStyle
                                          letterSpacing:(NSNumber *)letterSpacing
                                     useBackgroundColor:(BOOL)useBackgroundColor
                                        foregroundColor:(UIColor *)foregroundColor
                                        backgroundColor:(UIColor *)backgroundColor
                                                opacity:(CGFloat)opacity
{
  if (_fontSize && !isnan(_fontSize)) {
    fontSize = @(_fontSize);
  }
  if (_fontWeight) {
    fontWeight = _fontWeight;
  }
  if (_fontStyle) {
    fontStyle = _fontStyle;
  }
  if (_fontFamily) {
    fontFamily = _fontFamily;
  }
  if (!isnan(_letterSpacing)) {
    letterSpacing = @(_letterSpacing);
  }
  
  _effectiveLetterSpacing = letterSpacing.doubleValue;
  
  NSMutableAttributedString *attributedString = [NSMutableAttributedString new];
  [attributedString appendAttributedString:[[NSAttributedString alloc] initWithString:_text ?: @""]];
  
  
  [self _addAttribute:NSForegroundColorAttributeName
            withValue:[foregroundColor colorWithAlphaComponent:CGColorGetAlpha(foregroundColor.CGColor) * opacity]
   toAttributedString:attributedString];
  
  
  if (useBackgroundColor && backgroundColor) {
    [self _addAttribute:NSBackgroundColorAttributeName
              withValue:[backgroundColor colorWithAlphaComponent:CGColorGetAlpha(backgroundColor.CGColor) * opacity]
     toAttributedString:attributedString];
  }
  
   UIFont *font = [RCTFont updateFont:nil withFamily:fontFamily size:fontSize weight:fontWeight style:fontStyle variant:nil scaleMultiplier:(_allowFontScaling && _fontSizeMultiplier > 0.0 ? _fontSizeMultiplier : 1.0)];
 
  [self _addAttribute:NSFontAttributeName withValue:font toAttributedString:attributedString];
  [self _addAttribute:NSKernAttributeName withValue:letterSpacing toAttributedString:attributedString];
  [self _addAttribute:VRTLabelReactTagAttributeName withValue:self.reactTag toAttributedString:attributedString];
  [self _setParagraphStyleOnAttributedString:attributedString];
  
  _cachedMutableAttributedString = attributedString;
  return _cachedMutableAttributedString;
}

- (void)_addAttribute:(NSString *)attribute withValue:(id)attributeValue toAttributedString:(NSMutableAttributedString *)attributedString
{
  [attributedString enumerateAttribute:attribute inRange:NSMakeRange(0, attributedString.length) options:0 usingBlock:^(id value, NSRange range, BOOL *stop) {
    if (!value && attributeValue) {
      [attributedString addAttribute:attribute value:attributeValue range:range];
    }
  }];
}

/*
 * LineHeight works the same way line-height works in the web: if children and self have
 * varying lineHeights, we simply take the max.
 */
- (void)_setParagraphStyleOnAttributedString:(NSMutableAttributedString *)attributedString
{
  // check if we have lineHeight set on self
  __block BOOL hasParagraphStyle = NO;
  if (_lineHeight || _textAlign) {
    hasParagraphStyle = YES;
  }
  
  if (!_lineHeight) {
    self.lineHeight = 0.0;
  }

  // check for lineHeight on each of our children, update the max as we go (in self.lineHeight)
  [attributedString enumerateAttribute:NSParagraphStyleAttributeName inRange:(NSRange){0, attributedString.length} options:0 usingBlock:^(id value, NSRange range, BOOL *stop) {
    if (value) {
      NSParagraphStyle *paragraphStyle = (NSParagraphStyle *)value;
      CGFloat maximumLineHeight = round(paragraphStyle.maximumLineHeight / self.fontSizeMultiplier);
      if (maximumLineHeight > self.lineHeight) {
        self.lineHeight = maximumLineHeight;
      }
      hasParagraphStyle = YES;
    }
  }];
  
  if(!_textAlign) {
    self.textAlign = NSTextAlignmentNatural;
  }
  
  if(!_writingDirection) {
    self.writingDirection = NSWritingDirectionNatural;
  }
  
  // if we found anything, set it :D
  if (hasParagraphStyle) {
    NSMutableParagraphStyle *paragraphStyle = [NSMutableParagraphStyle new];
    paragraphStyle.alignment = _textAlign;
    paragraphStyle.baseWritingDirection = _writingDirection;
    CGFloat lineHeight = round(_lineHeight * (_allowFontScaling && self.fontSizeMultiplier > 0.0 ? self.fontSizeMultiplier : 1.0));
    paragraphStyle.minimumLineHeight = lineHeight;
    paragraphStyle.maximumLineHeight = lineHeight;
    [attributedString addAttribute:NSParagraphStyleAttributeName
                             value:paragraphStyle
                             range:(NSRange){0, attributedString.length}];
  }
  
  // Text decoration
  if(_textDecorationLine == RCTTextDecorationLineTypeUnderline ||
     _textDecorationLine == RCTTextDecorationLineTypeUnderlineStrikethrough) {
    [self _addAttribute:NSUnderlineStyleAttributeName withValue:@(_textDecorationStyle)
     toAttributedString:attributedString];
  }
  if(_textDecorationLine == RCTTextDecorationLineTypeStrikethrough ||
     _textDecorationLine == RCTTextDecorationLineTypeUnderlineStrikethrough){
    [self _addAttribute:NSStrikethroughStyleAttributeName withValue:@(_textDecorationStyle)
     toAttributedString:attributedString];
  }
  if(_textDecorationColor) {
    [self _addAttribute:NSStrikethroughColorAttributeName withValue:_textDecorationColor
     toAttributedString:attributedString];
    [self _addAttribute:NSUnderlineColorAttributeName withValue:_textDecorationColor
     toAttributedString:attributedString];
  }
}

// Sets the center position of the text.
- (void)setPosition:(NSArray<NSNumber *> *)position {
  _position = [position copy];
  float positionValues[3];
  populateFloatArrayFromNSArray(position, positionValues, 3);
  [self node]->setPosition({positionValues[0], positionValues[1], positionValues[2]});
}


- (void)setRotation:(NSArray<NSNumber *> *)rotation{
  _rotation = [rotation copy];
  float rotationValues[3];
  populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
  // TODO self.vroUIView.vroLayer->setRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
}

- (void)setScale:(NSArray<NSNumber *> *)scale
{
  float scaleValues[3];
  populateFloatArrayFromNSArray(scale, scaleValues, 3);
  [self node]->setScale({scaleValues[0], scaleValues[1], scaleValues[2]});
}

/*
 * Takes as input the bounds of the label and a boolean to indicate if the text is within a flexview
 * or not. The text is then properly sized within the given bounds, and then flagged for update
 * so it can render.
 */
- (void)sizeAndUpdateLabel:(CGRect)frame isInFlexView:(BOOL)isInFlexView {

  // TODO if(!self.label) {
  // TODO  self.label = [[UILabel alloc] initWithFrame:self.vroUIView.bounds];
  // TODO }
  
  if(!self.color) {
    self.color = [UIColor blackColor];
  }
  
  BOOL useBackgroundColor = YES;
  if(!self.backgroundColor) {
    useBackgroundColor = NO;
  // TODO   [self.vroUIView setBackgroundColor:[UIColor clearColor]];
  // TODO   [self.label setBackgroundColor:[UIColor clearColor]];
  }

  if(frame.size.width <= 0 || frame.size.height <=0) {
    // Set width and height to 1 to prevent crash.. Prevents crash with iOS CALayer. This effectively allows us to silently fail if the user does something funky within a flexbox layout that gets to state like this. This can occur if the padding on a flexview is as thick as the flexview length, leaving no room for child views.
    frame = CGRectMake(frame.origin.x, frame.origin.y, 1, 1);
  }
  // Create the NSAttributed
  NSAttributedString *attributedText = [self _attributedStringWithFontFamily:nil
                                                                    fontSize:nil
                                                                  fontWeight:nil
                                                                   fontStyle:nil
                                                               letterSpacing:nil
                                                          useBackgroundColor:useBackgroundColor
                                                             foregroundColor:self.color
                                                             backgroundColor:self.backgroundColor
                                                                     opacity:1.0];
  
  
  
  CGRect paragraphRect;
  if(!isInFlexView) {
    // Fine out the estimated width and height of the given label.
    paragraphRect = [attributedText boundingRectWithSize:CGSizeMake(kTextPointsPerSpatialCoord, CGFLOAT_MAX)
                                                 options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading)
                                                 context:nil];
    
    // The label will only take up it's space, while the parent UIView will take up kPointsPerSpatialCoord and the label height.
    // TODO [self.vroUIView setFrame:CGRectMake(0, 0, kTextPointsPerSpatialCoord, paragraphRect.size.height)];
    CGFloat xOffset = kTextPointsPerSpatialCoord/2 - paragraphRect.size.width/2;
    // TODO [self.label setFrame:CGRectMake(xOffset, 0, paragraphRect.size.width, paragraphRect.size.height)];
  } else {
    
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)attributedText);
    
    CGFloat scaleWidth = frame.size.width/kTextPointsPerSpatialCoord;
    CGFloat scaleHeight = frame.size.height/kTextPointsPerSpatialCoord;
    
    CGSize targetSize = CGSizeMake(kTextPointsPerSpatialCoord, CGFLOAT_MAX);
    CGSize fitSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, [attributedText length]), NULL, targetSize, NULL);
    CFRelease(framesetter);
    
    // For spatial panels, the width of the parent frame will always be kPointsPerSpatialCoord.
    // The height will also be set to kPointsPerSpatialPanel times the height to width ratio of the panel frame. This ensures that as the panel scales, the text will always be the same size.
    float heightToWidthRatio = scaleHeight / scaleWidth;
    // TODO [self.vroUIView setFrame:CGRectMake(0, 0, kTextPointsPerSpatialCoord, kTextPointsPerSpatialCoord*heightToWidthRatio)];
    // TODO [self.label setFrame:CGRectMake(0, 0, fitSize.width, fitSize.height)];
    
    // Readjust frame if the label is too small to take up entire length of parent view and is meant to be center or right aligned.
    if(fitSize.width < kTextPointsPerSpatialCoord) {
      if(self.textAlign == NSTextAlignmentRight) {
        CGFloat xOffset = kTextPointsPerSpatialCoord - fitSize.width;
       // TODO  [self.label setFrame:CGRectMake(xOffset, 0, fitSize.width, fitSize.height)];
      }
      else if(self.textAlign == NSTextAlignmentCenter) {
        CGFloat xOffset = kTextPointsPerSpatialCoord/2 - fitSize.width/2;
        // TODO [self.label setFrame:CGRectMake(xOffset, 0, fitSize.width, fitSize.height)];
      }
    }
  }
  
  if(self.maxLines > 1) {
    // TODO [self.label setNumberOfLines:self.maxLines];
    // TODO self.label.lineBreakMode = NSLineBreakByWordWrapping;
  }else{
    // Auto wrap the text.
    // TODO self.label.lineBreakMode = NSLineBreakByWordWrapping;
    // TODO [self.label setNumberOfLines:0];
  }
  
  // TODO [self.label setAttributedText:attributedText];
  // TODO [self.vroUIView addSubview:self.label];
  
  // TODO CGRect fittedFrame = self.label.frame;
  // TODO float ratio = fittedFrame.size.height/fittedFrame.size.width;
  // TODO _ratio = ratio;
  
  // TODO self.vroUIView.vroLayer->setOpacity(1);
  _boundsCalculated = YES;

  if(!isInFlexView) {
    // Have the 3d coord scale for text be width(default 1.0 unit) by height(height = label height/kPointsPerSpatialCoord. This ensures all text is same width and height regardless of length of text.
    // TODO _ratio = fittedFrame.size.height/kTextPointsPerSpatialCoord;
    // Offset the position when setting the frame, since frame takes the bottom position of the text.
    // TODO self.vroUIView.vroLayer->setFrame(VRORectMake([_position[0] floatValue] - _width/2.0f,[_position[1] floatValue] - (_width * _ratio)/2.0f,[_position[2] floatValue], _width, _width * _ratio));
  } else {
    [super reactSetFrame:frame];
  }
  if(self.driver) {
    // TODO [self.vroUIView updateWithDriver:self.driver];
  }
  else {
    // When viewDidAppear is invoked, the driver will be avail and we can update then.
    _needsUpdating = YES;
  }

}

// Called when the text is part of a flexbox layout.
- (void)reactSetFrame:(CGRect)frame {
  _frame = frame;
  [self sizeAndUpdateLabel:frame isInFlexView:true];
}

// Method invoked before being added to scene, meant to overridden by subclasses.
- (void)viewWillAppear {
  [super viewWillAppear];
  BOOL isFlexboxComponent = [self isWithinFlexView];
 
  if(_needsUpdating) {
    // TODO [self.vroUIView updateWithDriver:self.driver];
    _needsUpdating = NO;
  }
  
  if(_boundsCalculated == NO && (isFlexboxComponent == NO)) {
    _frame = CGRectMake(0, 0,  kTextPointsPerSpatialCoord, kTextPointsPerSpatialCoord);
    [self sizeAndUpdateLabel:_frame isInFlexView:isFlexboxComponent];
  }
}

- (BOOL)isWithinFlexView {
  if(self.superview && [self.superview isKindOfClass:[VRTFlexView class]]) {
    return YES;
  }
  return NO;
}

-(CGPoint)fudgeFlexboxScaleX:(float)width3d  Y:(float)height3d {
  
  return CGPointMake(width3d, height3d);
}

//override node to return vroLayer of vroUIView
- (std::shared_ptr<VRONode>) node {
  // TODO return self.vroUIView.vroLayer;
    return {};
}



@end
