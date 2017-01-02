//
//  VROSceneDelegateiOS.h
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSceneDelegateiOS_h
#define VROSceneDelegateiOS_h
#import <Foundation/Foundation.h>

/**
 * Protocol to be implemented by objective C controls to be
 * set on VROSceneControllerDelegateiOS for the notification of
 * scene transition events.
 */
@protocol VROSceneDelegateProtocol<NSObject>
- (void)sceneWillAppear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)sceneDidAppear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)sceneWillDisappear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)sceneDidDisappear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)startIncomingTransition:(VRORenderContext *)context duration:(float)duration;
- (void)startOutgoingTransition:(VRORenderContext *)context duration:(float)duration;
- (void)endIncomingTransition:(VRORenderContext *)context;
- (void)endOutgoingTransition:(VRORenderContext *)context;
- (void)animateIncomingTransition:(VRORenderContext *)context percentComplete:(float)t;
- (void)animateOutgoingTransition:(VRORenderContext *)context percentComplete:(float)t;
@end

/**
 * iOS implementation of VROSceneControllerDelegate for the notification
 * of scene delegate events across the bridge.
 */
class VROSceneControllerDelegateiOS : public VROSceneController::VROSceneControllerDelegate {
public:
    VROSceneControllerDelegateiOS(id<VROSceneDelegateProtocol> delegate) :
        _delegate(delegate) {}
    virtual ~VROSceneControllerDelegateiOS() {}

    /*
     Scene appeared delegate methods.
     */
    virtual void onSceneWillAppear(VRORenderContext *context, VRODriver *driver) {
        [_delegate sceneWillAppear:context driver:driver];
    }
    virtual void onSceneDidAppear(VRORenderContext *context, VRODriver *driver) {
        [_delegate sceneDidAppear:context driver:driver];
    }
    virtual void onSceneWillDisappear(VRORenderContext *context, VRODriver *driver) {
        [_delegate sceneWillDisappear:context driver:driver];
    }
    virtual void onSceneDidDisappear(VRORenderContext *context, VRODriver *driver) {
        [_delegate sceneDidDisappear:context driver:driver];
    }

    /*
     Scene animation delegate methods.
     */
    virtual void startIncomingTransition(VRORenderContext *context, float duration){
        [_delegate startIncomingTransition:context duration:duration];
    }
    virtual void startOutgoingTransition(VRORenderContext *context, float duration){
       [_delegate startOutgoingTransition:context duration:duration];
    }
    virtual void endIncomingTransition(VRORenderContext *context) {
        [_delegate endIncomingTransition:context];
    }
    virtual void endOutgoingTransition(VRORenderContext *context) {
        [_delegate endOutgoingTransition:context];
    }
    virtual void animateIncomingTransition(VRORenderContext *context, float t){
        [_delegate animateIncomingTransition:context percentComplete:t];
    }
    virtual void animateOutgoingTransition(VRORenderContext *context, float t){
        [_delegate animateOutgoingTransition:context percentComplete:t];
    }

private:
    id<VROSceneDelegateProtocol> _delegate;
};
#endif /* VROSceneDelegateiOS_h */
