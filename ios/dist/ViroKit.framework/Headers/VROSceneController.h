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

static float kTransitionPushDistance = 0.10;

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
    }
    virtual ~VROSceneController() {}

    std::shared_ptr<VROScene> getScene() {
        return _scene;
    }

    /*
     Scene appeared delegate methods, triggered by VRORenderer.
     */
    void onSceneWillAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->onSceneWillAppear(context, driver);
    }
    void onSceneDidAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->onSceneDidAppear(context, driver);
    }
    void onSceneWillDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }

        _sceneDelegateWeak.lock()->onSceneWillDisappear(context, driver);
    }
    void onSceneDidDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        if (_sceneDelegateWeak.expired()){
            return;
        }
        
        _sceneDelegateWeak.lock()->onSceneDidDisappear(context, driver);
    }

    // For now, we use fade plus push for all animated scene transitions. TODO: VIRO-771
    // captures applying varying kinds of custom animations on scene transitions.
    void startIncomingTransition(float duration, VROTimingFunctionType timingFunctionType,
                                 VRORenderContext *context) {
        
        VROVector3f forward = context->getCamera().getForward().scale(kTransitionPushDistance);
        
        // Preserve the current opacity and position of root nodes. We start each node off at
        // opacity 0 and will animate toward this preserved opacity
        std::map<std::shared_ptr<VRONode>, float> preservedOpacities;
        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            preservedOpacities[root] = root->getOpacity();
            root->setOpacity(0.0f);
            
            // Start each root node back 1 unit. The nodes will animate toward
            // the camera. This also eliminates z-fighting with the outgoing scene
            root->setPosition(root->getPosition() + forward);
        }

        if (_scene->getBackground() != nullptr) {
            _scene->getBackground()->getMaterials().front()->setTransparency(0.0);
        }

        // Construct and commit fade-in/pull-in animation for the scene
        VROTransaction::begin();
        VROTransaction::setAnimationDuration(duration);
        VROTransaction::setTimingFunction(timingFunctionType);

        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            root->setOpacity(preservedOpacities[root]);
            root->setPosition(root->getPosition() - forward);
        }

        if (_scene->getBackground() != nullptr) {
            _scene->getBackground()->getMaterials().front()->setTransparency(1.0);
        }

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

    void startOutgoingTransition(float duration, VROTimingFunctionType timingFunctionType,
                                 VRORenderContext *context) {
        
        VROVector3f forward = context->getCamera().getForward().scale(kTransitionPushDistance);

        // Preserve the current opacity of root nodes. When the scene disappears, we'll
        // restore that opacity (so that the next time the scene appears, it will be at said
        // previous opacity).
        std::map<std::shared_ptr<VRONode>, float> preservedOpacities;
        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            preservedOpacities[root] = root->getOpacity();
        }

        // Construct and commit fade-out/push-back animation for the scene
        VROTransaction::begin();
        VROTransaction::setAnimationDuration(duration);
        VROTransaction::setTimingFunction(timingFunctionType);

        for (std::shared_ptr<VRONode> root : _scene->getRootNodes()) {
            root->setOpacity(0.0f);
            root->setPosition(root->getPosition() + forward);
        }

        if (_scene->getBackground() != nullptr) {
            _scene->getBackground()->getMaterials().front()->setTransparency(0.0);
        }
        
        // At the end of the animation, restore the opacity and position of the
        // nodes (since they are no longer visible)
        std::weak_ptr<VROSceneController> weakPtr = shared_from_this();
        VROTransaction::setFinishCallback([weakPtr, preservedOpacities, forward]() {
            std::shared_ptr<VROSceneController> scene = weakPtr.lock();
            if (scene) {
                for (std::shared_ptr<VRONode> root : scene->getScene()->getRootNodes()) {
                    auto it = preservedOpacities.find(root);
                    if (it != preservedOpacities.end()) {
                        root->setOpacity(it->second);
                    }
                    
                    root->setPosition(root->getPosition() - forward);
                }
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

};

#endif //ANDROID_VROSCENECONTROLLER_H
