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
- (void)onTrackingInitialized;
- (void)onAmbientLightUpdate:(float)intensity colorTemperature:(float)colorTemperature;
- (void)onAnchorFound:(std::shared_ptr<VROARAnchor>)anchor;
- (void)onAnchorUpdated:(std::shared_ptr<VROARAnchor>)anchor;
- (void)onAnchorRemoved:(std::shared_ptr<VROARAnchor>)anchor;
@end

class VROARSceneDelegateiOS : public VROARSceneDelegate, public VROARDeclarativeSessionDelegate {
public:
    VROARSceneDelegateiOS(id<VROARSceneDelegateProtocol> delegate) :
        _delegate(delegate) {}
    
    virtual ~VROARSceneDelegateiOS() {}

    virtual void onTrackingInitialized() {
        [_delegate onTrackingInitialized];
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

    virtual void onAmbientLightUpdate(float intensity, float colorTemperature) {
        [_delegate onAmbientLightUpdate:intensity colorTemperature:colorTemperature];
    }

private:
    __weak id<VROARSceneDelegateProtocol> _delegate;
};
#endif /* VROARSceneDelegateiOS_h */
