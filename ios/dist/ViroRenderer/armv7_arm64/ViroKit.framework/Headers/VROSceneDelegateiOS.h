//
//  VROSceneDelegateiOS.h
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROSceneDelegateiOS_h
#define VROSceneDelegateiOS_h
#import <Foundation/Foundation.h>

/**
 * Protocol to be implemented by objective C controls to be
 * set on VROSceneControllerDelegateiOS for the notification of
 * scene transition events.
 */
@protocol VROSceneDelegateProtocol<NSObject>
- (void)sceneWillAppear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver;
- (void)sceneDidAppear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver;
- (void)sceneWillDisappear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver;
- (void)sceneDidDisappear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver;
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
    virtual void onSceneWillAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        [_delegate sceneWillAppear:context driver:driver];
    }
    virtual void onSceneDidAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        [_delegate sceneDidAppear:context driver:driver];
    }
    virtual void onSceneWillDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        [_delegate sceneWillDisappear:context driver:driver];
    }
    virtual void onSceneDidDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
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
  
    __weak id<VROSceneDelegateProtocol> _delegate;
  
};
#endif /* VROSceneDelegateiOS_h */
