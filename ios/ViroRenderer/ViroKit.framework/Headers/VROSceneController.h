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

class VROScene;
class VROVector3f;
class VRODriver;
class VRORenderContext;

/*
 * TODO VIRO-708: Remove Scene Controller in favor of VROScene. With a
 * SceneDelegate that both ios (VROSceneDelegateiOS) and Java (VROSceneDelegateJNI)
 * Will extend from. Scene changes will then be notified through its Delegate.
 */
class VROSceneController : public std::enable_shared_from_this<VROSceneController>{

public:
    // Delegate for callbacks across the bridge
    class VROSceneControllerDelegate {
    public:
        virtual void onSceneWillAppear(VRORenderContext * context, VRODriver *driver){};
        virtual void onSceneDidAppear(VRORenderContext * context, VRODriver *driver){};
        virtual void onSceneWillDisappear(VRORenderContext * context, VRODriver *driver){};
        virtual void onSceneDidDisappear(VRORenderContext * context, VRODriver *driver){};
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
     Scene appeared delegate methods, triggered by VRORenderer.
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

    // For now, we use fading for all animated scene transitions. TODO: VIRO-771
    // captures applying varying kinds of custom animations on scene transitions.
    void startIncomingTransition(float duration, VROTimingFunctionType timingFunctionType){
        // Set the scene and background items to be invisible.
        for(std::shared_ptr<VRONode> root :  _scene->getRootNodes()) {
            // Save a copy of the scene's initial default opacity,
            // to be used for restoring to.
            _sceneInitialOpacity[root] = root->getOpacity();
            root->setOpacity(0.0f);
        }

        if (_scene->getBackground() != nullptr) {
            _scene->getBackground()->getMaterials().front()->setTransparency(0.0);
        }

        // Construct and commit fade-in animation for the scene
        VROTransaction::begin();
        VROTransaction::setAnimationDuration(duration);
        VROTransaction::setTimingFunction(timingFunctionType);

        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            root->setOpacity(_sceneInitialOpacity[root]);
        }

        if (_scene->getBackground() != nullptr) {
            _scene->getBackground()->getMaterials().front()->setTransparency(1.0);
        }

        commitAnimation();
    }

    void startOutgoingTransition(float duration, VROTimingFunctionType timingFunctionType){
        // Refresh known opacity of root nodes, so when we come back to this scene we know what they should be.
        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            _sceneInitialOpacity[root] = root->getOpacity();
        }

        // Construct and commit fade-out animation for the scene
        VROTransaction::begin();
        VROTransaction::setAnimationDuration(duration);
        VROTransaction::setTimingFunction(timingFunctionType);

        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            root->setOpacity(0.0f);
        }

        if (_scene->getBackground() != nullptr) {
            _scene->getBackground()->getMaterials().front()->setTransparency(0.0);
        }

        commitAnimation();
    }

    void commitAnimation(){
        // Set callback delegates
        std::weak_ptr<VROSceneController> weakPtr = shared_from_this();
        VROTransaction::setFinishCallback([weakPtr]() {
            std::shared_ptr<VROSceneController> scene = weakPtr.lock();
            if (scene) {
                scene->setActiveTransitionAnimation(false);
            }
        });

        setActiveTransitionAnimation(true);
        VROTransaction::commit();
    }

    bool hasActiveTransitionAnimation(){
        return _isTransitionAnimationActive;
    }

    void setActiveTransitionAnimation (bool flag){
        _isTransitionAnimationActive = flag;
    }

    /*
     Per-frame rendering delegate methods.
     */
    void sceneWillRender(const VRORenderContext *context) {}
private:
    std::shared_ptr<VROScene> _scene;
    std::weak_ptr<VROSceneControllerDelegate> _sceneDelegateWeak;

    bool _isTransitionAnimationActive;
    std::map<std::shared_ptr<VRONode>, float> _sceneInitialOpacity;

};

#endif //ANDROID_VROSCENECONTROLLER_H
