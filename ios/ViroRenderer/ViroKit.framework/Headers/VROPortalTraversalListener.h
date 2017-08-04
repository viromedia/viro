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
class VROLineSegment;

class VROPortalTraversalListener : public VROFrameListener {
public:
    
    VROPortalTraversalListener(std::shared_ptr<VROScene> scene);
    virtual ~VROPortalTraversalListener();
    
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);
    
private:
    
    std::weak_ptr<VROScene> _scene;
    
    std::shared_ptr<VROPortal> findPortalTraversal(const VROLineSegment &segment,
                                                   const tree<std::shared_ptr<VROPortal>> &portalTree);
    
};

#endif /* VROPortalTraversalListener_h */
