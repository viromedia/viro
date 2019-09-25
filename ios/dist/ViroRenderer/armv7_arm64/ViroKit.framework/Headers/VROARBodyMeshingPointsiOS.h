//
//  VROARBodyMeshingPointsiOS.h
//  ViroKit
//
//  Created by vik.advani on 9/4/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARBodyMeshingPointsiOS_h
#define VROARBodyMeshingPointsiOS_h

#include <memory>
#include "VROCameraTexture.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>

class VRODriver;

enum class VROBodyMeshingJoints {
        kTop = 0,
        kNeck = 1,
        kRightShoulder = 2,
        kRightElbow = 3,
        kRightWrist = 4,
        kLeftShoulder = 5,
        kLeftElbow =6,
        kLeftWrist = 7,
        kRightHip = 8,
        kRightKnee = 9,
        kRightAnkle = 10,
        kLeftHip = 11,
        kLeftKnee = 12,
        kLeftAngle = 13,
    
 };

@interface VROBodyMeshingCaptureDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate> {
    CMTime _lastTimestamp;
    int32_t _fps;
    VNCoreMLRequest *_coreMLRequest;
}

- (id)initWithMLRequest:(VNCoreMLRequest *)coreMLRequest;

@end

class BodyPoint  {
public:
    BodyPoint(CGPoint point, double confidence) {
        _point = point;
        _confidence = confidence;
    }

    CGPoint _point;
    double _confidence;
};

@interface BodyPointImpl: NSObject
    @property (readwrite, nonatomic) CGPoint _point;
    @property (readwrite, nonatomic) double _confidence;
@end

class VROARBodyMeshingPointsiOS {
    
    public:
        class VROBodyMeshingDelegate {
            public:
                virtual void onBodyMeshJointsAvail(NSDictionary *joints) {};
        };
    
    
        VROARBodyMeshingPointsiOS();
        bool initBodyTracking(VROCameraPosition position, std::shared_ptr<VRODriver> driver);
        bool isBodyPointConfidenceLessThan(BodyPointImpl *bodyPoint, float confidence);
        void startBodyTracking();
        void stopBodyTracking();
        void processBuffer(CVPixelBufferRef sampleBuffer);
        uint8_t ClampIntToByte(int n);
        uint8_t clamp(int16_t input);
        void printBodyPoint(NSDictionary *bodyPoints, VROBodyMeshingJoints jointType);
        void writeImageToDisk(CVPixelBufferRef imageBuffer);
        CVPixelBufferRef convertImage(CVImageBufferRef imageBuffer);
        NSDictionary  * convert(MLMultiArray *heatmap);

        void setDelegate(std::shared_ptr<VROBodyMeshingDelegate> delegate) {
            auto autoWeakDelegate = delegate;
            _bodyMeshDelegateWeak = autoWeakDelegate;
        }
    private:
        /*
         Capture session and delegate used for live video playback.
         */
        AVCaptureSession *_captureSession;
        MLModel *_model;
        VNCoreMLModel *_coreMLModel;
        VNCoreMLRequest *_coreMLRequest;
        CMTime _lastTimestamp;
        int32_t _fps;
        VROBodyMeshingCaptureDelegate *_delegate;
        dispatch_queue_t bodyMeshingQueue;
        std::weak_ptr<VROBodyMeshingDelegate> _bodyMeshDelegateWeak;
        CVPixelBufferRef rotatedBuffer;
};

#endif


