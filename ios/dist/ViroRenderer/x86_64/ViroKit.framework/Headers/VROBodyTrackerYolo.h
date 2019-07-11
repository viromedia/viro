//
//  VROBodyTrackerYolo.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/16/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROBodyTrackerYolo_h
#define VROBodyTrackerYolo_h

#include "VROBodyTracker.h"
#include <map>
#include "VROCameraTexture.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>
#include "VROMatrix4f.h"

class VRODriver;

class VROBodyTrackerYolo : public VROBodyTracker {
    
public:
    
    VROBodyTrackerYolo();
    virtual ~VROBodyTrackerYolo() {}
    
    bool initBodyTracking(VROCameraPosition position, std::shared_ptr<VRODriver> driver);
    void startBodyTracking();
    void stopBodyTracking();
    
    void update(const VROARFrame *frame);
    
private:
    
    static std::map<std::string, VROBodyJointType> _labelsToJointTypes;
    
    MLModel *_model;
    VNCoreMLModel *_coreMLModel;
    VNCoreMLRequest *_visionRequest;
    
    dispatch_queue_t _visionQueue;
    VROMatrix4f _transform;
    CVPixelBufferRef _currentImage;
    
    void trackCurrentImage(VROMatrix4f transform, VROCameraOrientation orientation);
    void processVisionResults(VNRequest *request, NSError *error);
    
};

#endif /* VROBodyTrackerYolo_h */
