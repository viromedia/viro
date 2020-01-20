//
//  VROPortalDelegateiOS.h
//  ViroKit
//
//  Created by Vik Advani on 9/14/17.
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
