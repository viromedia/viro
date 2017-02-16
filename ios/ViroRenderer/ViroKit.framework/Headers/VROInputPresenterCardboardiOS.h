//
//  VROControllerPresenterCardboard.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROControllerPresenterCardboardiOS_H
#define VROControllerPresenterCardboardiOS_H

#include <memory>
#include <string>
#include <vector>
#include "VRORenderContext.h"
#include "VROInputControllerBase.h"
#include "VROEventDelegate.h"

class VROInputPresenterCardboardiOS : public VROInputPresenter {
public:
    VROInputPresenterCardboardiOS(std::shared_ptr<VRORenderContext> context):VROInputPresenter(context) {
        setReticle(std::make_shared<VROReticle>(nullptr));
        getReticle()->setPointerMode(false);
    }
    virtual ~VROInputPresenterCardboardiOS() {}

    void onClick(int source, ClickState clickState) {
        if (clickState==ClickState::ClickUp){
            getReticle()->trigger();
        }
    }
    void onGazeHit(int source, float distance, VROVector3f hitLocation) {
        VROInputPresenter::onReticleGazeHit(distance, hitLocation);
     }
};
#endif
