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
#import <Foundation/Foundation.h>

@protocol VROARSceneDelegateProtocol <NSObject>
@required
- (void)onTrackingInitialized;
- (void)onAmbientLightUpdate:(float)intensity colorTemperature:(float)colorTemperature;
- (void)onAnchorFound:(std::shared_ptr<VROARAnchor>)anchor;
- (void)onAnchorUpdated:(std::shared_ptr<VROARAnchor>)anchor;
- (void)onAnchorRemoved:(std::shared_ptr<VROARAnchor>)anchor;
@end

/**
 * iOS implementation of the VROARSceneDelegate
 */
class VROARSceneDelegateiOS : public VROARSceneDelegate {
public:
    VROARSceneDelegateiOS(id<VROARSceneDelegateProtocol> delegate) :
        _delegate(delegate) {}
    
    virtual ~VROARSceneDelegateiOS() {}

    virtual void onTrackingInitialized() {
        [_delegate onTrackingInitialized];
    }

    virtual void onAmbientLightUpdate(float intensity, float colorTemperature) {
        [_delegate onAmbientLightUpdate:intensity colorTemperature:colorTemperature];
    }

    virtual void onAnchorFound(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onAnchorFound:anchor];
    }

    virtual void onAnchorUpdated(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onAnchorUpdated:anchor];
    }

    virtual void onAnchorRemoved(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onAnchorRemoved:anchor];
    }
private:
    __weak id<VROARSceneDelegateProtocol> _delegate;
};
#endif /* VROARSceneDelegateiOS_h */
