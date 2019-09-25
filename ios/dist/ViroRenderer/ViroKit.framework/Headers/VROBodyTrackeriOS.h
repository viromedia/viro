//
//  VROBodyTrackeriOS.h
//  ViroKit
//
//  Created by vik.advani on 9/4/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROBodyTrackeriOS_h
#define VROBodyTrackeriOS_h

#include <memory>
#include "VROBodyTracker.h"
#include "VROCameraTexture.h"
#include "VROVisionEngine.h"
#import <Foundation/Foundation.h>
#include "VROMatrix4f.h"

class VRODriver;
class VROPoseFilter;
class VROVisionEngine;

class API_AVAILABLE(ios(11.0)) VROBodyTrackeriOS : public VROBodyTracker, public VROVisionEngineDelegate {
public:
    
    VROBodyTrackeriOS();
    virtual ~VROBodyTrackeriOS();
    
    bool initBodyTracking(VROCameraPosition position, std::shared_ptr<VRODriver> driver);
    void startBodyTracking();
    void stopBodyTracking();
    void update(const VROARFrame *frame);
    
    /*
     Process the output of the VROVisionEngine. This will convert the raw output from CoreML into the
     body joints, pass the joints through a filter and finally invoke the delegate on the rendering
     thread.
     
     Invoked on the visionQueue.
     */
    std::vector<std::pair<VROVector3f, float>> processVisionOutput(VNCoreMLFeatureValueObservation *result, VROCameraPosition cameraPosition,
                                                                   VROMatrix4f visionToImageSpace, VROMatrix4f imageToViewportSpace);
    
    /*
     Sets the window period at which we sample points for dampening. If period == 0,
     no dampening will be applied.
     */
    void setDampeningPeriodMs(double period);
    double getDampeningPeriodMs() const;
    
    /*
     Get the dynamic crop box used for the last render.
     */
    CGRect getDynamicCropBox() const {
        return _visionEngine->getDynamicCropBox();
    }
    
    /*
     Get the filter being used to smooth the output joint data.
     */
    std::shared_ptr<VROPoseFilter> getPoseFilter() {
        return _poseFilter;
    }
    
private:
    
    /*
     Handles processing of AR frames through CoreML and back out into this object
     for handling.
     */
    std::shared_ptr<VROVisionEngine> _visionEngine;
    
    /*
     True when tracking is running; e.g. images are being fed into CoreML.
     */
    bool _isTracking;

    /*
     Filter used on pose data before sending to the delegate.
     */
    std::shared_ptr<VROPoseFilter> _poseFilter;
    
    /*
     Dampening window milliseconds. If period is set to 0, no dampening will be applied.
     */
    double _dampeningPeriodMs;
    
    /*
     Converts the heatmap output from the given CoreML MLMultiArray into body joints in screen
     coordinates. The given transforms go from vision space [0, 1] to image space [0, 1], to
     normalized viewport space [0, 1].
     */
    static VROPoseFrame convertHeatmap(MLMultiArray *heatmap, VROCameraPosition cameraPosition,
                                       VROMatrix4f visionToImageSpace, VROMatrix4f imageToViewportSpace,
                                       std::pair<VROVector3f, float> *outImageSpaceJoints);
    
};

#endif


