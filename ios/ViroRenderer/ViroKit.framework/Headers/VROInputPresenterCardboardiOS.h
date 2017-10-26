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
#include "VROHitTestResult.h"

class VROInputPresenterCardboardiOS : public VROInputPresenter {
public:
    
    VROInputPresenterCardboardiOS() {
        setReticle(std::make_shared<VROReticle>(nullptr));
        getReticle()->setPointerMode(false);
    }
    virtual ~VROInputPresenterCardboardiOS() {}

    void onClick(int source, std::shared_ptr<VRONode> node, ClickState clickState, std::vector<float> clickedPosition) {
        VROInputPresenter::onClick(source, node, clickState, clickedPosition);
        if (clickState == ClickState::ClickUp){
            getReticle()->trigger();
        }
    }
    
    void onGazeHit(int source, std::shared_ptr<VRONode> node, const VROHitTestResult &hit) {
        VROInputPresenter::onReticleGazeHit(hit);
    }
};

#endif
