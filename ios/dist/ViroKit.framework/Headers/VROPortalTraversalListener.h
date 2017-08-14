//
//  VROPortalTraversalListener.h
//  ViroKit
//
//  Created by Raj Advani on 8/3/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPortalTraversalListener_h
#define VROPortalTraversalListener_h

#include "VROFrameListener.h"
#include "VROTree.h"
#include <memory>

class VROScene;
class VROPortal;
class VROVector3f;
class VROLineSegment;

class VROPortalTraversalListener : public VROFrameListener {
public:
    
    VROPortalTraversalListener(std::shared_ptr<VROScene> scene);
    virtual ~VROPortalTraversalListener();
    
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);
    
private:
    
    std::weak_ptr<VROScene> _scene;
    
    /*
     Scan the portal tree to see if we've intersected any passable portal with the given
     segment. Return the portal found, if any.
     */
    std::shared_ptr<VROPortal> findPortalTraversal(const VROLineSegment &segment,
                                                   const tree<std::shared_ptr<VROPortal>> &portalTree);
    
    /*
     Scan the portal tree for passable portals, and if we're a significant distance
     away from them, restore them to one-sided mode. We only want portals we are about
     to traverse to be in two-sided mode.
     */
    void restorePortalFaces(const VROVector3f &cameraPosition,
                            const tree<std::shared_ptr<VROPortal>> &portalTree);
    
};

#endif /* VROPortalTraversalListener_h */
