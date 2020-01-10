//
//  VROARSceneDelegateiOS.h
//  ViroKit
//
//  Created by Andy Chu on 7/10/17.
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

#ifndef VROARSceneDelegateiOS_h
#define VROARSceneDelegateiOS_h

#import "VROARScene.h"
#import "VROARDeclarativeSession.h"
#import <Foundation/Foundation.h>

@protocol VROARSceneDelegateProtocol <NSObject>
@required
- (void)onTrackingUpdated:(VROARTrackingState)state withReason:(VROARTrackingStateReason)reason;
- (void)onAmbientLightUpdate:(float)intensity color:(VROVector3f)color;
- (void)onAnchorFound:(std::shared_ptr<VROARAnchor>)anchor;
- (void)onAnchorUpdated:(std::shared_ptr<VROARAnchor>)anchor;
- (void)onAnchorRemoved:(std::shared_ptr<VROARAnchor>)anchor;
@end

class VROARSceneDelegateiOS : public VROARSceneDelegate, public VROARDeclarativeSessionDelegate {
public:
    VROARSceneDelegateiOS(id<VROARSceneDelegateProtocol> delegate) :
        _delegate(delegate) {}
    
    virtual ~VROARSceneDelegateiOS() {}

    virtual void onTrackingUpdated(VROARTrackingState state, VROARTrackingStateReason reason) {
        [_delegate onTrackingUpdated:state withReason:reason];
    }
    
    virtual void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onAnchorFound:anchor];
    }
    
    virtual void anchorWillUpdate(std::shared_ptr<VROARAnchor> anchor) {
         
    }
    
    virtual void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor) {
         [_delegate onAnchorUpdated:anchor];
    }
    
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onAnchorRemoved:anchor];
    }

    virtual void onAmbientLightUpdate(float intensity, VROVector3f color) {
        [_delegate onAmbientLightUpdate:intensity color:color];
    }

private:
    __weak id<VROARSceneDelegateProtocol> _delegate;
};
#endif /* VROARSceneDelegateiOS_h */
