//
//  VROSceneController.h
//  ViroKit
//
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef ANDROID_VROSCENECONTROLLER_H
#define ANDROID_VROSCENECONTROLLER_H
#include <memory>
#include "VROScene.h"
#include "VROLog.h"

class VROScene;
class VROVector3f;
class VRODriver;
class VRORenderContext;

/*
 * TODO VIRO-708: Remove Scene Controller in favor of VROScene. With a
 * SceneDelegate that both ios (VROSceneDelegateiOS) and Java (VROSceneDelegateJNI)
 * Will extend from. Scene changes will then be notified through its Delegate.
 */
class VROSceneController {

public:
    // Delegate for callbacks across the bridge
    class VROSceneControllerDelegate {
    public:
        virtual void onSceneWillAppear(VRORenderContext * context, VRODriver *driver){};
        virtual void onSceneDidAppear(VRORenderContext * context, VRODriver *driver){};
        virtual void onSceneWillDisappear(VRORenderContext * context, VRODriver *driver){};
        virtual void onSceneDidDisappear(VRORenderContext * context, VRODriver *driver){};
        virtual void startIncomingTransition(VRORenderContext * context, float duration){};
        virtual void startOutgoingTransition(VRORenderContext * context, float duration){};
        virtual void endIncomingTransition(VRORenderContext * context) {};
        virtual void endOutgoingTransition(VRORenderContext * context) {};
        virtual void animateIncomingTransition(VRORenderContext * context, float t){};
        virtual void animateOutgoingTransition(VRORenderContext * context, float t){};
    };

    void setDelegate(std::shared_ptr<VROSceneControllerDelegate> delegate){
        auto autoWeakDelegate = delegate;
        _sceneDelegateWeak = autoWeakDelegate;
    }

    VROSceneController() {
        _scene = std::make_shared<VROScene>();
    }
    virtual ~VROSceneController() {}

    std::shared_ptr<VROScene> getScene() {
        return _scene;
    }

    /*
     Scene appeared delegate methods.
     */
    void onSceneWillAppear(VRORenderContext *context, VRODriver &driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->onSceneWillAppear(context, &driver);
    }
    void onSceneDidAppear(VRORenderContext *context, VRODriver &driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->onSceneDidAppear(context, &driver);
    }
    void onSceneWillDisappear(VRORenderContext *context, VRODriver &driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->onSceneWillDisappear(context, &driver);
    }
    void onSceneDidDisappear(VRORenderContext *context, VRODriver &driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }
        
        _sceneDelegateWeak.lock()->onSceneDidDisappear(context, &driver);
    }

    /*
     Scene animation delegate methods.
     */
    void startIncomingTransition(VRORenderContext *context, float duration){
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->startIncomingTransition(context, duration);
    }
    void startOutgoingTransition(VRORenderContext *context, float duration){
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->startOutgoingTransition(context, duration);
    }
    void endIncomingTransition(VRORenderContext *context) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->endIncomingTransition(context);
    }
    void endOutgoingTransition(VRORenderContext *context) {
        if (_sceneDelegateWeak.expired()){
            return;
        }
        
        _sceneDelegateWeak.lock()->endOutgoingTransition(context);
    }
    void animateIncomingTransition(VRORenderContext *context, float t){
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->animateIncomingTransition(context, t);
    }
    void animateOutgoingTransition(VRORenderContext *context, float t){
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->animateOutgoingTransition(context, t);
    }

    /*
     Per-frame rendering delegate methods.
     */
    void sceneWillRender(const VRORenderContext *context) {}

private:
    std::shared_ptr<VROScene> _scene;
    std::weak_ptr<VROSceneControllerDelegate> _sceneDelegateWeak;
};

#endif //ANDROID_VROSCENECONTROLLER_H
