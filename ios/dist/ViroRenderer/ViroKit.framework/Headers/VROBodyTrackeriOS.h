//
//  VROBodyTrackeriOS.h
//  ViroKit
//
//  Created by vik.advani on 9/4/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROBodyTrackeriOS_h
#define VROBodyTrackeriOS_h

#include "VROBodyTracker.h"
#include <map>
#include "VROCameraTexture.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>
#include "VROMatrix4f.h"

class VRODriver;

class VROBodyTrackeriOS : public VROBodyTracker {
    
public:
    
    VROBodyTrackeriOS();
    virtual ~VROBodyTrackeriOS() {}
    
    bool initBodyTracking(VROCameraPosition position, std::shared_ptr<VRODriver> driver);
    void startBodyTracking();
    void stopBodyTracking();
    
    void trackWithVision(CVPixelBufferRef image, VROMatrix4f transform, VROCameraOrientation orientation);
    
private:
    
    MLModel *_model;
    VNCoreMLModel *_coreMLModel;
    VNCoreMLRequest *_coreMLRequest;
    double _lastTimestamp;
    int32_t _fps;
    dispatch_queue_t bodyMeshingQueue;
    
    VROMatrix4f _transform;
    
    static CVPixelBufferRef convertImage(CVImageBufferRef image);
    static CVPixelBufferRef rotateImage(CVPixelBufferRef image, uint8_t rotation, size_t resultWidth, size_t resultHeight);
    static CVPixelBufferRef transformImage(CVPixelBufferRef image, CGAffineTransform transform);
    static std::map<VROBodyJointType, VROBodyJoint> convertHeatmap(MLMultiArray *heatmap, VROMatrix4f transform);
    
    // Debug method
    void writeImageToDisk(CVPixelBufferRef imageBuffer);

};

#endif


