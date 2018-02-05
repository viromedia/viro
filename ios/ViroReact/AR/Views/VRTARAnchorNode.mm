//
//  VRTARNode.mm
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTARAnchorNode.h"
#import "VRTARUtils.h"

@implementation VRTARAnchorNode

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROARNode>();
}

- (void)setIsAnchored:(BOOL)isAttached {
    _isAnchored = isAttached;
    [self handleAppearanceChange];
}

- (void)setAnchorId:(NSString *)anchorId {
    _anchorId = anchorId;
}

- (BOOL)shouldAppear {
    return [super shouldAppear] && self.isAnchored;
}

- (void)setPauseUpdates:(BOOL)pauseUpdates {
    _pauseUpdates = pauseUpdates;
    std::shared_ptr<VROARNode> arNode = std::dynamic_pointer_cast<VROARNode>([self node]);
    if (arNode) {
        arNode->setPauseUpdates(pauseUpdates);
    }
}

#pragma mark - VROARNodeDelegateProtocol Implementation
- (void)onARAnchorAttached:(std::shared_ptr<VROARAnchor>) anchor {
    self.isAnchored = true;
    [self handleAppearanceChange];
    if (_onAnchorFoundViro) {
        _onAnchorFoundViro(@{ @"anchorFoundMap" : [VRTARUtils createDictionaryFromAnchor:anchor]});
    }
}

- (void)onARAnchorUpdated:(std::shared_ptr<VROARAnchor>) anchor {
    if (_onAnchorUpdatedViro) {
        _onAnchorUpdatedViro(@{ @"anchorUpdatedMap" : [VRTARUtils createDictionaryFromAnchor:anchor]});
    }
}

- (void)onARAnchorRemoved {
    self.isAnchored = false;
    [self handleAppearanceChange];
    if (_onAnchorRemovedViro) {
        // we don't need to return any args for anchor removed
        _onAnchorRemovedViro(@{});
    }
}

@end
