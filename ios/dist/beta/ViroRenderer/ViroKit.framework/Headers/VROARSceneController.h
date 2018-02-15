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
    VROARSceneController();
    virtual ~VROARSceneController();
    
    virtual void onSceneWillAppear(VRORenderContext *context, std::shared_ptr<VRODriver> driver);
    virtual void onSceneWillDisappear(VRORenderContext *context, std::shared_ptr<VRODriver> driver);
};

#endif /* VROARSceneController_h */
