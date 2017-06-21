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

class VROARSceneController : public VROSceneController {
public:
    VROARSceneController() {
        _scene = std::make_shared<VROARScene>();
    }
    virtual ~VROARSceneController() {}
    
    virtual std::shared_ptr<VROScene> getScene() {
        return _scene;
    }
    
};

#endif /* VROARSceneController_h */
