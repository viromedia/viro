//
//  VRTPortalScene.m
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
