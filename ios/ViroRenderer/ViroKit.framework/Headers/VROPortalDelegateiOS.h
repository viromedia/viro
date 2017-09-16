//
//  VROPortalDelegateiOS.h
//  ViroKit
//
//  Created by Vik Advani on 9/14/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPortalDelegateiOS_h
#define VROPortalDelegateiOS_h
#import "VROPortalDelegate.h"
#import <Foundation/Foundation.h>

/*
 Protocol to be implemented by objective C controls to be
 set on VROEventDelegateiOS for the notification of
 input events.
 */
@protocol VROPortalDelegateProtocol<NSObject>
@required
- (void)onPortalEnter;
- (void)onPortalExit;
@end


/*
 iOS implementation of VROEventDelegate for the notification
 of delegate events across the bridge.
 */
class VROPortalDelegateiOS : public VROPortalDelegate  {
public:
    VROPortalDelegateiOS(id<VROPortalDelegateProtocol> delegate) :
    _delegate(delegate) {}
    virtual ~VROPortalDelegateiOS() {}
    
    /*
     Delegate events triggered by the EventManager.
     */
    virtual void onPortalEnter() {
        [_delegate onPortalEnter];
    }
    virtual void onPortalExit() {
        [_delegate onPortalExit];
    }
    
private:
    __weak id<VROPortalDelegateProtocol> _delegate;
    
};

#endif /* VROPortalDelegateiOS_h */
