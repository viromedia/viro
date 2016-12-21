//
//  VROSceneControllerInternal.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 3/25/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSceneControllerInternal_h
#define VROSceneControllerInternal_h

#include <memory>
#include "VROHoverDelegate.h"
#include "VROVector3f.h"

class VROScene;
class VRODriver;
class VRONode;
class VROReticle;
class VRORenderContext;
class VROHoverDelegate;
class VROHoverController;
class VROFrameSynchronizer;
class VROHoverDistanceListener;

class VROSceneControllerInternal : public VROHoverDelegate {
    
public:
    
    VROSceneControllerInternal();
    virtual ~VROSceneControllerInternal();
    
#pragma mark - Control methods
    
    void setHoverEnabled(bool enabled, bool boundsOnly);
    virtual bool isHoverTestBoundsOnly() const {
        return _hoverTestBoundsOnly;
    }

    std::shared_ptr<VROScene> getScene() const {
        return _scene;
    }
    
#pragma mark - Delegate methods
    
    /*
     Scene appeared delegate methods.
     */
    virtual void onSceneWillAppear(VRORenderContext &context, VRODriver &driver);
    virtual void onSceneDidAppear(VRORenderContext &context, VRODriver &driver);
    virtual void onSceneWillDisappear(VRORenderContext &context, VRODriver &driver);
    virtual void onSceneDidDisappear(VRORenderContext &context, VRODriver &driver);
    
    /*
     Scene animation delegate methods.
     */
    virtual void startIncomingTransition(VRORenderContext *context, float duration) = 0;
    virtual void startOutgoingTransition(VRORenderContext *context, float duration) = 0;
    virtual void endIncomingTransition(VRORenderContext *context) = 0;
    virtual void endOutgoingTransition(VRORenderContext *context) = 0;
    virtual void animateIncomingTransition(VRORenderContext *context, float t) = 0;
    virtual void animateOutgoingTransition(VRORenderContext *context, float t) = 0;
    
    /*
     Per-frame rendering delegate methods.
     */
    virtual void sceneWillRender(const VRORenderContext *context) = 0;
    
    /*
     Hover delegate methods.
     */
    virtual bool isHoverable(std::shared_ptr<VRONode> node) = 0;
    virtual void hoverOnNode(std::shared_ptr<VRONode> node) = 0;
    virtual void hoverOffNode(std::shared_ptr<VRONode> node) = 0;
    
    /*
     Reticle delegate methods.
     */
    virtual void reticleTapped(VROVector3f ray, const VRORenderContext *context) = 0;
    
#pragma mark - Internal
    
    void attach(std::shared_ptr<VROReticle> reticle,
                std::shared_ptr<VROFrameSynchronizer> frameSynchronizer);
    
private:
    
    /*
     The scene managed by this scene controller.
     */
    std::shared_ptr<VROScene> _scene;
    
    /*
     The hover controller used for this scene.
     */
    std::shared_ptr<VROHoverController> _hoverController;
    
    /*
     Used to add/remove frame listeners.
     */
    std::weak_ptr<VROFrameSynchronizer> _frameSynchronizer;
    
    /*
     True if the hover test should only use bounding boxes.
     */
    bool _hoverTestBoundsOnly;
    
    /*
     Listener that changes size of reticle based on what we're hovering.
     */
    std::shared_ptr<VROHoverDistanceListener> _reticleSizeListener;
};

#endif /* VROSceneControllerInternal_h */
