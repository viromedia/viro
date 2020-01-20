//
//  VROSceneController.h
//  ViroKit
//
//  Copyright © 2015 Viro Media. All rights reserved.
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
