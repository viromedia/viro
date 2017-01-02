//
//  VRORenderDelegateiOS.h
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROEventDelegateiOS_h
#define VROEventDelegateiOS_h
#import "VROEventDelegate.h"
#import <Foundation/Foundation.h>

/**
 * Protocol to be implemented by objective C controls to be
 * set on VROEventDelegateiOS for the notification of
 * input events.
 */
@protocol VROEventDelegateProtocol<NSObject>
@required
- (void)onTapped;
- (void)onGaze:(bool)isGazing;
@end

/**
 * iOS implementation of VROEventDelegate for the notification
 * of delegate events across the bridge.
 */
class VROEventDelegateiOS : public VROEventDelegate  {
public:
    VROEventDelegateiOS(id<VROEventDelegateProtocol> delegate) :
        _delegate(delegate) {}
    virtual ~VROEventDelegateiOS() {}

    /*
     * Delegate events triggered by the EventManager.
     */
    virtual void onTapped() {
        [_delegate onTapped];
    }
    virtual void onGaze(bool isGazing) {
        [_delegate onGaze:isGazing];
    }
    virtual void onGazeHitDistance(float distance) {
        //No-op
    }
private:
    id<VROEventDelegateProtocol> _delegate;
};
#endif /* VROEventDelegateiOS_h */
