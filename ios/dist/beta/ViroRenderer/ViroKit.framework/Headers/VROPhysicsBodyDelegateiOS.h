//
//  VROPhysicsBodyDelegateiOS.h
//  ViroRenderer
//
//  Copyright © 201Z Viro Media. All rights reserved.
//

#ifndef VROPhysicsBodyDelegateiOS_h
#define VROPhysicsBodyDelegateiOS_h
#import <Foundation/Foundation.h>
#import "VROPhysicsBodyDelegate.h"

/**
 * Protocol to be implemented by objective C controls to be
 * set on VROPhysicsBodyDelegateiOS for the notification of
 * physics events.
 */
@protocol VROPhysicsBodyDelegateProtocol<NSObject>
@required
- (void)onCollided:(std::string) bodyTagB
         collision:(VROPhysicsBody::VROCollision) collision;
@end

/**
 * iOS implementation of VROPhysicsBodyDelegate for the notification
 * of delegate events across the bridge.
 */
class VROPhysicsBodyDelegateiOS : public VROPhysicsBodyDelegate  {
public:
    VROPhysicsBodyDelegateiOS(id<VROPhysicsBodyDelegateProtocol> delegate) :
        _delegate(delegate) {}
    virtual ~VROPhysicsBodyDelegateiOS() {}
    
    virtual void onCollided(std::string bodyBKey, VROPhysicsBody::VROCollision collision) {
        [_delegate onCollided:bodyBKey collision:collision];
    }
    
private:
    __weak id<VROPhysicsBodyDelegateProtocol> _delegate;
  
};
#endif /* VROEventDelegateiOS_h */
