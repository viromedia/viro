//
//  VROARSceneDelegateiOS.h
//  ViroKit
//
//  Created by Andy Chu on 7/10/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
