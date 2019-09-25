//
//  VROPoseFilterEuro.h
//  ViroKit
//
//  Created by Raj Advani on 3/1/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROPoseFilterEuro_h
#define VROPoseFilterEuro_h

#include "VROPoseFilter.h"
#include "VROOneEuroFilter.h"

class VROPoseFilterEuro : public VROPoseFilter {
public:
    
    VROPoseFilterEuro(float trackingPeriodMs, float confidenceThreshold);
    virtual ~VROPoseFilterEuro();
    
    VROPoseFrame temporalFilter(const std::vector<VROPoseFrame> &frames, const VROPoseFrame &combinedFrame,
                                const VROPoseFrame &newFrame);
    
    /*
     Set the parameters to use for *all* joints. See VROPoseFilterEuro.cpp top-notes
     for a description of these parameters.
     */
    void setBeta(float beta);
    void setFCMin(float fcMin);
    
private:
    std::vector<std::shared_ptr<VROOneEuroFilter>> _filters;
    
};

#endif /* VROPoseFilterEuro_h */
