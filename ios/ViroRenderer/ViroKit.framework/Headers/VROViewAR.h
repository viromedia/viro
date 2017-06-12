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

class VROARSessionDelegate;

@interface VROViewAR : GLKView <VROView>

@property (readwrite, nonatomic) BOOL suspended;

- (void)setARSessionDelegate:(std::shared_ptr<VROARSessionDelegate>)delegate;

@end
