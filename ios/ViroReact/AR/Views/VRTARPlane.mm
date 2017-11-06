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

@implementation VRTARPlane {
    bool _shouldUpdate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        // default min height/width is 0, we match any surface
        _minWidth = kARPlaneDefaultMinWidth;
        _minHeight = kARPlaneDefaultMinHeight;
        _pauseUpdates = NO;
        _arNodeDelegate = std::make_shared<VROARNodeDelegateiOS>(self);
        
        std::shared_ptr<VROARDeclarativePlane> plane = std::dynamic_pointer_cast<VROARDeclarativePlane>([self node]);
        plane->setARNodeDelegate(_arNodeDelegate);
    }
    return self;
}

- (std::shared_ptr<VROARDeclarativeSession>)declarativeSession {
    return std::dynamic_pointer_cast<VROARScene>([self scene])->getDeclarativeSession();
}

- (void)parentDidDisappear {
    if ([self scene]) {
        [self declarativeSession]->removeARNode(std::dynamic_pointer_cast<VROARDeclarativePlane>(self.node));
    }
    [super parentDidDisappear];
}

- (void)setAnchorId:(NSString *)anchorId {
    [super setAnchorId:anchorId];
    _shouldUpdate = true;
    std::shared_ptr<VROARDeclarativePlane> plane = std::dynamic_pointer_cast<VROARDeclarativePlane>([self node]);
    if (plane) {
        plane->setAnchorId(std::string([self.anchorId UTF8String]));
    }
}

- (void)setScene:(std::shared_ptr<VROScene>)scene {
    [self declarativeSession]->addARNode(std::dynamic_pointer_cast<VROARDeclarativePlane>(self.node));
    [super setScene:scene];
}

- (void)setMinHeight:(float)minHeight {
    _minHeight = minHeight;
    _shouldUpdate = true;
    std::shared_ptr<VROARDeclarativePlane> plane = std::dynamic_pointer_cast<VROARDeclarativePlane>([self node]);
    if (plane) {
        plane->setMinHeight(_minHeight);
    }
}

- (void)setMinWidth:(float)minWidth {
    _minWidth = minWidth;
    _shouldUpdate = true;
    std::shared_ptr<VROARDeclarativePlane> plane = std::dynamic_pointer_cast<VROARDeclarativePlane>([self node]);
    if (plane) {
        plane->setMinWidth(_minWidth);
    }
}

- (void)setPauseUpdates:(BOOL)pauseUpdates {
    _pauseUpdates = pauseUpdates;
    std::shared_ptr<VROARDeclarativePlane> plane = std::dynamic_pointer_cast<VROARDeclarativePlane>([self node]);
    if (plane) {
        plane->setPauseUpdates(pauseUpdates);
    }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (_shouldUpdate) {
        std::shared_ptr<VROARScene> arScene = std::dynamic_pointer_cast<VROARScene>(self.scene);
        if (arScene) {
            [self declarativeSession]->updateARNode(std::dynamic_pointer_cast<VROARDeclarativePlane>(self.node));
            _shouldUpdate = false;
        }
    }
}

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROARDeclarativePlane>(kARPlaneDefaultMinWidth, kARPlaneDefaultMinHeight);
}

@end
