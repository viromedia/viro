//
//  VRTPortal.h
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTNode.h"

@interface VRTPortalFrame : VRTNode

- (std::shared_ptr<VROPortalFrame>)portalFrame;

@end
