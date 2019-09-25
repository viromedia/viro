//
//  VRTPortal.h
//  ViroReact
//
//  Created by Raj Advani on 8/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTNode.h"

@interface VRTPortalScene : VRTNode<VROPortalDelegateProtocol>

@property (readwrite, nonatomic) BOOL passable;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onPortalEnterViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onPortalExitViro;

- (std::shared_ptr<VROPortal>)portal;

@property (readwrite, nonatomic) std::shared_ptr<VROPortalDelegateiOS> portalDelegate;


@end
