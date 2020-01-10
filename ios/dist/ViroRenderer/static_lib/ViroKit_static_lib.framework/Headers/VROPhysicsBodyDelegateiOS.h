//
//  VROPhysicsBodyDelegateiOS.h
//  ViroRenderer
//
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
