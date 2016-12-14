//
//  VRTButton.h
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTImage.h"

@interface VRTButton : VRTImage

-(instancetype)initWithBridge:(RCTBridge *)bridge;
/**
 * Original scale representing the default button state (not tapped / highlighted).
 **/
@property (nonatomic) VROVector3f defaultScale;

@property (nonatomic, copy) RCTImageSource *gazeSource;
@property (nonatomic, copy) RCTImageSource *tapSource;

/**
 * Called when a user depresses a taps down over an element.
 */
- (void)onTapDown;

/**
 * Called when a user releases a tap down action over an element.
 */
- (void)onTapUp;

/**
 * Called when a user gazes over an element with his reticle.
 */
- (void)onGazeStart;

/**
 * Called when a user's reticle gaze leaves an element.
 */
- (void)onGazeEnd;

@end
