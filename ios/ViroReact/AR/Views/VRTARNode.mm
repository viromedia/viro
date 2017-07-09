//
//  VRTARNode.mm
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTARNode.h"

@implementation VRTARNode

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROARNode>();
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
    return @{};
}

@end
