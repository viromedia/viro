//
//  VROARFrameInertial.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARFrameInertial_h
#define VROARFrameInertial_h

#include "VROARFrame.h"
#include "VROCameraTexture.h"
#include "VROViewport.h"
#import <AVFoundation/AVFoundation.h>

class VROARFrameInertial : public VROARFrame {
public:
    
    VROARFrameInertial(const std::shared_ptr<VROARCamera> &camera, VROViewport viewport);
    virtual ~VROARFrameInertial();
    
    CMSampleBufferRef getImage() const;
    CGImagePropertyOrientation getImageOrientation() const;
    
    double getTimestamp() const;
    VROCameraOrientation getOrientation() const { return VROCameraOrientation::Portrait; }
    
    const std::shared_ptr<VROARCamera> &getCamera() const;
    std::vector<std::shared_ptr<VROARHitTestResult>> hitTest(int x, int y, std::set<VROARHitTestResultType> types);
    std::vector<std::shared_ptr<VROARHitTestResult>> hitTestRay(VROVector3f *origin,
                                                              VROVector3f *destination ,
                                                                std::set<VROARHitTestResultType> types) {
      // no-op
      std::vector<std::shared_ptr<VROARHitTestResult>> emptyResults;
      return emptyResults;
    }
    VROMatrix4f getViewportToCameraImageTransform() const;

    const std::vector<std::shared_ptr<VROARAnchor>> &getAnchors() const;
    
    float getAmbientLightIntensity() const;
    VROVector3f getAmbientLightColor() const;

    std::shared_ptr<VROARPointCloud> getPointCloud();
    
private:
    
    const std::shared_ptr<VROARCamera> _camera;
    double _timestamp;
    VROViewport _viewport;
    std::vector<std::shared_ptr<VROARAnchor>> _anchors;
    
};

#endif /* VROARFrameInertial_h */
