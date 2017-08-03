//
//  VRTARDraggableNode.mm
//  ViroReact
//
//  Created by Andy Chu on 8/3/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTARDraggableNode.h"

@implementation VRTARDraggableNode

- (std::shared_ptr<VRONode>)createVroNode {
    return std::make_shared<VROARDraggableNode>();
}

@end
