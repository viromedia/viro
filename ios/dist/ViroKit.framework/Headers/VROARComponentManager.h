//
//  VROARComponentManager.h
//  ViroKit
//
//  Created by Andy Chu on 6/16/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARComponentManager_h
#define VROARComponentManager_h

#include "VROARSession.h"
#include "VROARPlane.h"

/*
 This class contains all the attach and detach logic between virtual nodes and real anchors
 that make up an AR component.
 */
class VROARComponentManager : public VROARSessionDelegate {
public:
    VROARComponentManager() {};
    virtual ~VROARComponentManager() {};
    
    // TODO: possibly update these functions to take in generic VROARNode objects
    void addARPlane(std::shared_ptr<VROARPlane> plane);
    void removeARPlane(std::shared_ptr<VROARPlane> plane);
    void updateARPlane(std::shared_ptr<VROARPlane> plane);
    
    /*
     This function should be called when all the planes are being removed
     */
    void clearAllPlanes(std::vector<std::shared_ptr<VROARPlane>> planes);
    
    // VROARSessionDelegate functions
    virtual void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWillUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
    
private:
    // Anchors without an associated Node
    std::vector<std::shared_ptr<VROARAnchor>> _detachedAnchors;
    // Planes without an associated Anchor
    std::vector<std::shared_ptr<VROARPlane>> _detachedPlanes;
    
    /*
     Returns a detached anchor that is suitable for the given plane
     */
    std::shared_ptr<VROARAnchor> findDetachedAnchor(std::shared_ptr<VROARPlane> plane);
    
    /*
     Returns a detached plane satisfied by the given anchor
     */
    std::shared_ptr<VROARPlane> findDetachedPlane(std::shared_ptr<VROARAnchor> anchor);
    
    /*
     This functions either attaches the detached plane to an anchor or adds the plane to
     _detachedPlanes
     */
    void handleDetachedPlane(std::shared_ptr<VROARPlane> plane);
    
    /*
     This functions either attaches the detached anchor to an plane or adds the anchor to
     _detachedAnchor
     */
    void handleDetachedAnchor(std::shared_ptr<VROARAnchor> anchor);

};

#endif /* VROARComponentManager_h */


