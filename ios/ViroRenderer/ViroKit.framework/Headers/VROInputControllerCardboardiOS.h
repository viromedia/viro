//
//  VROInputControllerCardboardiOS.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROInputControllerCardboardiOS_H
#define VROInputControllerCardboardiOS_H

#include <memory>
#include "VRORenderContext.h"
#include "VROInputControllerBase.h"
#include "VROInputPresenterCardboardiOS.h"

class VROInputControllerCardboardiOS : public VROInputControllerBase {

public:
    VROInputControllerCardboardiOS(){}
    virtual ~VROInputControllerCardboardiOS(){}
    void onProcess();
    void onScreenClicked();
    std::string getHeadset();
    std::string getController();

protected:
    std::shared_ptr<VROInputPresenter> createPresenter(std::shared_ptr<VRORenderContext> context){
        return std::make_shared<VROInputPresenterCardboardiOS>(context);
    }

private:
    void updateOrientation();
};
#endif
