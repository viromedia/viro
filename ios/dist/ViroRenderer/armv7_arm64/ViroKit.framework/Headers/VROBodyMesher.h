//
//  VROBodyMesher.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/23/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

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
