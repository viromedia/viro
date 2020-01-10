//
//  VROBodyMesher.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/23/19.
//  Copyright © 2019 Viro Media. All rights reserved.
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

#ifndef VROBodyMesher_h
#define VROBodyMesher_h

#include "VROVisionModel.h"

enum class VROCameraPosition;
class VRODriver;
class VROGeometry;

/*
 Delegate invoked each time the body mesh is updated.
 */
class VROBodyMesherDelegate {
public:
    virtual void onBodyMeshUpdated(const std::vector<float> &vertices, std::shared_ptr<VROGeometry> mesh) = 0;
};

/*
 Tracks and meshes the primary body in the camera view, creating a VROGeometry
 out of the result.
 */
class VROBodyMesher : public VROVisionModel {
    
public:
    VROBodyMesher() {};
    virtual ~VROBodyMesher() {}
    
    /*
     Lifecycle methods.
     */
    virtual bool initBodyTracking(VROCameraPosition position, std::shared_ptr<VRODriver> driver) = 0;
    virtual void startBodyTracking() = 0;
    virtual void stopBodyTracking() = 0;
    
    /*
     Set a delegate to receive body mesh updates.
     */
    void setDelegate(std::shared_ptr<VROBodyMesherDelegate> delegate) {
        _bodyMeshDelegate_w = delegate;
    }
    
    /*
     Get the body mesh as tracked by this controller. This mesh is updated continually.
     */
    virtual std::shared_ptr<VROGeometry> getBodyMesh() = 0;
    
    /*
     Sets the window period at which we sample points for dampening. If period == 0,
     no dampening will be applied.
     */
    virtual void setDampeningPeriodMs(double period) {}
    virtual double getDampeningPeriodMs() const { return 0; }
    
protected:
    std::weak_ptr<VROBodyMesherDelegate> _bodyMeshDelegate_w;
    
};

#endif /* VROBodyMesher_h */
