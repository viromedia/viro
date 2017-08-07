//
//  VRTPortal.m
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTPortalFrame.h"

@interface VRTPortalFrame ()

@end

@implementation VRTPortalFrame

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        
    }
    return self;
}

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROPortalFrame>();
}

- (std::shared_ptr<VROPortalFrame>)portalFrame {
    return std::dynamic_pointer_cast<VROPortalFrame>(self.node);
}

@end
