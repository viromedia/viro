//
//  VROARFrameiOS.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARFrameiOS_h
#define VROARFrameiOS_h

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110000
#include "VROARFrame.h"
#include "VROViewport.h"
#include <ARKit/ARKit.h>

class VROARSessioniOS;

class VROARFrameiOS : public VROARFrame {
public:
    
    VROARFrameiOS(ARFrame *frame, VROViewport viewport, VROCameraOrientation orientation,
                  std::shared_ptr<VROARSessioniOS> session);
    virtual ~VROARFrameiOS();
    
    CVPixelBufferRef getImage() const;
    CGImagePropertyOrientation getImageOrientation() const;

    double getTimestamp() const;
    VROCameraOrientation getOrientation() const { return _orientation; }
    
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
    
    ARFrame *_frame;
    VROViewport _viewport;
    VROCameraOrientation _orientation;
    std::weak_ptr<VROARSessioniOS> _session;
    std::shared_ptr<VROARCamera> _camera;
    std::vector<std::shared_ptr<VROARAnchor>> _anchors;
    std::shared_ptr<VROARPointCloud> _pointCloud;
    
};

#endif
#endif /* VROARFrameiOS_h */
