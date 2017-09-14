//
//  VRTPortal.m
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTPortal.h"
#import "VRT3DObject.h"
#import <React/RCTLog.h>

@interface VRTPortal ()

@end

@implementation VRTPortal

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        
    }
    return self;
}

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
    if(![subview isKindOfClass:[VRT3DObject class]]) {
           RCTLogError(@"Only a Viro3DObject can be a child of ViroPortal.");
    }
 
    [super insertReactSubview:subview atIndex:atIndex];
}
- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROPortalFrame>();
}

- (std::shared_ptr<VROPortalFrame>)portalFrame {
    return std::dynamic_pointer_cast<VROPortalFrame>(self.node);
}

@end
