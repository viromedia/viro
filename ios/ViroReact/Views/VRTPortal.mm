//
//  VRTPortal.m
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTPortal.h"
#import "VRTPortalFrame.h"

@interface VRTPortal ()

@end

@implementation VRTPortal

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        
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
    if ([view isKindOfClass:[VRTPortalFrame class]]) {
        VRTPortalFrame *frameView = (VRTPortalFrame *)view;
        [self portal]->setPortalEntrance([frameView portalFrame]);
    }
    [super insertReactSubview:view atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
    if ([subview isKindOfClass:[VRTPortalFrame class]]) {
        VRTPortalFrame *frameView = (VRTPortalFrame *)subview;
        [frameView clearPhysicsBody];
        
        [self portal]->setPortalEntrance({});
    }
    [super removeReactSubview:subview];
}

@end
