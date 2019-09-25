//
//  VRTPortalScene.m
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
//

#import "VRTPortalScene.h"
#import "VRTPortal.h"

@interface VRTPortalScene ()

@end

@implementation VRTPortalScene

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _portalDelegate = std::make_shared<VROPortalDelegateiOS>(self);
        [self portal]->setPortalDelegate(_portalDelegate);
    }
    return self;
}

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROPortal>();
}

- (std::shared_ptr<VROPortal>)portal {
    return std::dynamic_pointer_cast<VROPortal>(self.node);
}

- (void)setPassable:(BOOL)passable {
    [self portal]->setPassable(passable);
}

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
    if ([view isKindOfClass:[VRTPortal class]]) {
        VRTPortal *frameView = (VRTPortal *)view;
        [self portal]->setPortalEntrance([frameView portalFrame]);
    }
    [super insertReactSubview:view atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
    if ([subview isKindOfClass:[VRTPortal class]]) {
        VRTPortal *frameView = (VRTPortal *)subview;
        [frameView clearPhysicsBody];
        
        [self portal]->setPortalEntrance({});
    }
    [super removeReactSubview:subview];
}

#pragma mark - VROPortalDelegateProtocol Implementations

- (void)onPortalEnter {
    if (self.onPortalEnterViro) {
        self.onPortalEnterViro(nil);
    }
}

- (void)onPortalExit {
    if (self.onPortalExitViro) {
        self.onPortalExitViro(nil);
    }
}

@end
