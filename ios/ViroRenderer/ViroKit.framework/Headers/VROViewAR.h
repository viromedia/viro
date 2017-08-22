//
//  VROViewAR.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/31/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import "VROView.h"
#import "VROARSession.h"
#import "VROARHitTestResult.h"

// TODO VIRO-1355 Remove after event delegate integration
typedef void (^VROViewARTapHandler)(VROARHitTestResult, std::shared_ptr<VROARSession>, std::shared_ptr<VROScene>);

class VROARSessionDelegate;

@interface VROViewAR : GLKView <VROView, UIGestureRecognizerDelegate>

@property (readwrite, nonatomic) BOOL suspended;

- (void)setARSessionDelegate:(std::shared_ptr<VROARSessionDelegate>)delegate;

/*
 Sets the paused state of the underlying CADisplayLink
 */
- (void)setPaused:(BOOL)paused;

/*
 Performs an AR hit test with the given ray assuming origin is the camera
 */
- (std::vector<VROARHitTestResult>)performARHitTest:(VROVector3f)ray;

// TODO VIRO-1355 Remove after event delegate integration
@property (readwrite, nonatomic) VROViewARTapHandler tapHandler;

@end
