//
//  VRTARNode.mm
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTARAnchorNode.h"

@implementation VRTARAnchorNode

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROARNode>();
}

- (void)setIsAnchored:(BOOL)isAttached {
    _isAnchored = isAttached;
    [self handleAppearanceChange];
}

- (BOOL)shouldAppear {
    return [super shouldAppear] && self.isAnchored;
}

#pragma mark - VROARNodeDelegateProtocol Implementation
- (void)onARAnchorAttached:(std::shared_ptr<VROARAnchor>) anchor {
    self.isAnchored = true;
    [self handleAppearanceChange];
    if (_onAnchorFoundViro) {
        _onAnchorFoundViro(@{ @"anchorFoundMap" : [self createDictionaryFromAnchor:anchor]});
    }
}

- (void)onARAnchorUpdated:(std::shared_ptr<VROARAnchor>) anchor {
    if (_onAnchorUpdatedViro) {
        _onAnchorUpdatedViro(@{ @"anchorUpdatedMap" : [self createDictionaryFromAnchor:anchor]});
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

- (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor {
    return @{};
}

@end
