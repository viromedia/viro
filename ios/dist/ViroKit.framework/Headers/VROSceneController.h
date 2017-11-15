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
#include "VROTransaction.h"
#include "VROMaterial.h"
#include "VROPortal.h"

class VROScene;
class VROVector3f;
class VRODriver;
class VRORenderContext;
class VROPortalTraversalListener;

static float kTransitionPushDistance = 0.10;

/*
 SceneController manages transitions between Scenes and the Scene lifecyle.
 */
class VROSceneController : public std::enable_shared_from_this<VROSceneController>{

public:
    // Delegate for callbacks across the bridge
    class VROSceneControllerDelegate {
    public:
        virtual void onSceneWillAppear(VRORenderContext * context, std::shared_ptr<VRODriver> driver) {};
        virtual void onSceneDidAppear(VRORenderContext * context, std::shared_ptr<VRODriver> driver) {};
        virtual void onSceneWillDisappear(VRORenderContext * context, std::shared_ptr<VRODriver> driver) {};
        virtual void onSceneDidDisappear(VRORenderContext * context, std::shared_ptr<VRODriver> driver) {};
    };

    void setDelegate(std::shared_ptr<VROSceneControllerDelegate> delegate){
        auto autoWeakDelegate = delegate;
        _sceneDelegateWeak = autoWeakDelegate;
    }

    VROSceneController() {
        _scene = std::make_shared<VROScene>();
        _scene->getRootNode()->setScene(_scene, true);
    }
    virtual ~VROSceneController() {}

    virtual std::shared_ptr<VROScene> getScene() {
        return _scene;
    }

    /*
     Scene appeared delegate methods, triggered by VRORenderer.
     */
    virtual void onSceneWillAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver);
    virtual void onSceneDidAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver);
    virtual void onSceneWillDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver);
    virtual void onSceneDidDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver);

    // For now, we use fade plus push for all animated scene transitions. TODO: VIRO-771
    // captures applying varying kinds of custom animations on scene transitions.
    void startIncomingTransition(float duration, VROTimingFunctionType timingFunctionType,
                                 VRORenderContext *context);
    void startOutgoingTransition(float duration, VROTimingFunctionType timingFunctionType,
                                 VRORenderContext *context);

    bool hasActiveTransitionAnimation(){
        return _isTransitionAnimationActive;
    }
    void setActiveTransitionAnimation (bool flag){
        _isTransitionAnimationActive = flag;
    }
    
protected:
    
    std::shared_ptr<VROScene> _scene;
    std::weak_ptr<VROSceneControllerDelegate> _sceneDelegateWeak;

private:
    
    bool _isTransitionAnimationActive;
    std::shared_ptr<VROPortalTraversalListener> _portalTraversalListener;

};

#endif //ANDROID_VROSCENECONTROLLER_H
