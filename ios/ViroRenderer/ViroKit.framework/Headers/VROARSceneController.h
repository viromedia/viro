//
//  VROARSceneController.h
//  ViroKit
//
//  Created by Andy Chu on 6/14/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARSceneController_h
#define VROARSceneController_h

#include "VROSceneController.h"
#include "VROARScene.h"
#include "VROARDeclarativeSession.h"

class VROARSceneController : public VROSceneController {
public:
    VROARSceneController() {
        std::shared_ptr<VROARScene> scene = std::make_shared<VROARScene>();
        _scene = scene;
        _scene->getRootNode()->setScene(_scene, true);
    }
    virtual ~VROARSceneController() {}
    
    virtual std::shared_ptr<VROScene> getScene() {
        return _scene;
    }
    
    virtual void onSceneWillAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        VROSceneController::onSceneWillAppear(context, driver);
        std::shared_ptr<VROARScene> arScene = std::dynamic_pointer_cast<VROARScene>(_scene);
        if (arScene) {
            arScene->willAppear();
        }
    }
    
    virtual void onSceneWillDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver) {
        VROSceneController::onSceneWillDisappear(context, driver);
        std::shared_ptr<VROARScene> arScene = std::dynamic_pointer_cast<VROARScene>(_scene);
        if (arScene) {
            arScene->willDisappear();
        }
    }
};

#endif /* VROARSceneController_h */
