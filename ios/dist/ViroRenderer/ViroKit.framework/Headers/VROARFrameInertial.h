//
//  VROARFrameInertial.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
