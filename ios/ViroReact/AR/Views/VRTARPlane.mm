//
//  VRTARPlane.mm
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTARPlane.h"

static float const kARPlaneDefaultMinHeight = 0;
static float const kARPlaneDefaultMinWidth = 0;

@implementation VRTARPlane

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _minWidth = kARPlaneDefaultMinWidth;
        _minHeight = kARPlaneDefaultMinHeight;
    }
    return self;
}

- (void)setMinHeight:(float)minHeight {
    _minHeight = minHeight;
    std::shared_ptr<VROARPlane> plane = std::dynamic_pointer_cast<VROARPlane>([self node]);
    if (plane) {
        plane->setMinHeight(_minHeight);
    }
}

- (void)setMinWidth:(float)minWidth {
    _minWidth = minWidth;
    std::shared_ptr<VROARPlane> plane = std::dynamic_pointer_cast<VROARPlane>([self node]);
    if (plane) {
        plane->setMinWidth(_minWidth);
    }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    // TODO: notify ARScene or ARComponentManager if the width/height changed (VIRO-1260 required for interaction)
}

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROARPlane>(kARPlaneDefaultMinWidth, kARPlaneDefaultMinHeight);
}

@end
