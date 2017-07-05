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
        _arNodeDelegate = std::make_shared<VROARNodeDelegateiOS>(self);
        
        std::shared_ptr<VROARPlane> plane = std::dynamic_pointer_cast<VROARPlane>([self node]);
        plane->setARNodeDelegate(_arNodeDelegate);
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

#pragma mark - VROARNodeDelegateProtocol Implementation
- (void)onARAnchorAttached:(std::shared_ptr<VROARAnchor>) anchor {
    if (_onComponentFoundViro) {
        _onComponentFoundViro(@{ @"componentFoundMap" : [self createDictionaryFromAnchor:anchor]});
    }
}

- (void)onARAnchorUpdated:(std::shared_ptr<VROARAnchor>) anchor {
    if (_onComponentUpdatedViro) {
        _onComponentUpdatedViro(@{ @"componentUpdatedMap" : [self createDictionaryFromAnchor:anchor]});
    }
}

- (void)onARAnchorRemoved {
    if (_onComponentRemovedViro) {
        // we don't need to return any args for anchor removed
        _onComponentRemovedViro(@{});
    }
}

- (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor {
    std::shared_ptr<VROARPlaneAnchor> planeAnchor = std::dynamic_pointer_cast<VROARPlaneAnchor>(anchor);
    VROVector3f center = planeAnchor->getCenter();
    VROMatrix4f transform = planeAnchor->getTransform();
    VROQuaternion rotation = transform.extractRotation(transform.extractScale());
    VROVector3f extent = planeAnchor->getExtent();
    return @{
             @"position" : @[@(center.x), @(center.y), @(center.z)],
             @"rotation" : @[@(rotation.X), @(rotation.Y), @(rotation.Z)],
             @"width" : @(extent.x),
             @"height" : @(extent.z)
             };
}

@end
