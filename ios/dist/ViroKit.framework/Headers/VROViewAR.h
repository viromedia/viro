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

@interface VROViewAR : GLKView <VROView>

@property (readwrite, nonatomic) BOOL suspended;

- (void)setARSessionDelegate:(std::shared_ptr<VROARSessionDelegate>)delegate;

// TODO VIRO-1355 Remove after event delegate integration
@property (readwrite, nonatomic) VROViewARTapHandler tapHandler;

@end
