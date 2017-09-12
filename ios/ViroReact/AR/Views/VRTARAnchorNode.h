//
//  VRTARNode.h
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTNode.h"

/**
 * Base class for all Nodes that will be attached to an anchor
 */
@interface VRTARAnchorNode : VRTNode<VROARNodeDelegateProtocol>

@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorFoundViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorUpdatedViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorRemovedViro;

/*
 True/false if the AR node is attached to some anchor in the real world.
 */
@property (nonatomic, assign) BOOL isAnchored;

- (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor;

@end
