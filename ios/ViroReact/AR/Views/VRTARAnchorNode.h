//
//  VRTARAnchorNode.h
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

/*
 The ID of the underlying anchor this should attach to
 */
@property (nonatomic, copy, nullable) NSString *anchorId;

/*
 Determines whether or not this node should respond to transform updates
 from the underlying AR system.
 */
@property (nonatomic, assign) BOOL pauseUpdates;


@end
