//
//  VROScene.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/19/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROScene_h
#define VROScene_h

#include <stdio.h>
#include <vector>
#include <memory>
#include "VROInputPresenter.h"
#include "VROAllocationTracker.h"
#include "VROSortKey.h"
#include "VROQuaternion.h"
#include "VROThreadRestricted.h"
#include "VROPhysicsWorld.h"

class VRONode;
class VRORenderContext;
class VRODriver;
class VROTexture;
class VROGeometry;
class VROHitTestResult;
class VROVector3f;
class VROVector4f;
class VROAudioPlayer;
class VROInputControllerBase;

class VROScene : public std::enable_shared_from_this<VROScene>, public VROThreadRestricted {
    
public:
    
    VROScene();
    virtual ~VROScene();
    
    /*
     Render the backgrounds of the scene. This renders the background
     for each node in the scene that has one.
     */
    void renderBackground(const VRORenderContext &context,
                          std::shared_ptr<VRODriver> &driver);
    
    /*
     Render the visible nodes in this scene's graph, using their
     latest computed transforms and in an order determined by their
     latest computed sort keys.
     */
    void render(const VRORenderContext &context,
                std::shared_ptr<VRODriver> &driver);
    
    /*
     Compute the transforms, recursively, for all nodes in this scene.
     */
    void computeTransforms(const VRORenderContext &context);

    /*
     Update the visibility status of all nodes in the scene graph.
     */
    void updateVisibility(const VRORenderContext &context);
    
    /*
     Apply transformation constraints (e.g. billboarding) to all nodes in
     the scene.
     */
    void applyConstraints(const VRORenderContext &context);
    
    /*
     Update the sort keys for all nodes in this scene.
     */
    void updateSortKeys(const VRORenderContext &context,
                        std::shared_ptr<VRODriver> &driver);
    
    /*
     Render all portals in the tree (nodes with a portal bits that have 
     geometries) to the stencil buffer.
     */
    void renderStencil(const VRORenderContext &context,
                       std::shared_ptr<VRODriver> &driver);

    
    /*
     Retrieve the root node of the scene.
     */
    std::shared_ptr<VRONode> &getRootNode() {
        return _rootNode;
    }
    
    /*
     Returns the associated physics world with this scene. If there's none
     one is created and returned.
     */
    std::shared_ptr<VROPhysicsWorld> getPhysicsWorld() {
        if (_physicsWorld == nullptr) {
            _physicsWorld = std::make_shared<VROPhysicsWorld>();
        }
        return _physicsWorld;
    }

    /*
     Grabs and computes a physics world if one exists.
     */
    void computePhysics(const VRORenderContext &context) {
        if (_physicsWorld != nullptr) {
            _physicsWorld->computePhysics(context);
        }
    }
    
    /*
     Attach or detach input controllers.
     */
    void attachInputController(std::shared_ptr<VROInputControllerBase> controller);
    void detachInputController(std::shared_ptr<VROInputControllerBase> controller);
    std::shared_ptr<VROInputPresenter> getControllerPresenter();
    
    float getDistanceOfFurthestObjectFromCamera() const {
        return _distanceOfFurthestObjectFromCamera;
    }
    
    /*
     Get all backgrounds in the scene.
     */
    std::vector<std::shared_ptr<VROGeometry>> getBackgrounds();

protected:
    
    /*
     The root node of the scene.
     */
    std::shared_ptr<VRONode> _rootNode;

    /*
     UI representation of the underlying controller
     */
    std::shared_ptr<VROInputPresenter> _controllerPresenter;
    
    /*
     The nodes ordered for rendering by their sort keys.
     */
    std::vector<VROSortKey> _keys;
    
    /*
     The distance from the camera of the furthest away object, since the last
     call to updateSortKeys. Distance is from the camera to the bounding
     box of the object.
     */
    float _distanceOfFurthestObjectFromCamera;

    std::shared_ptr<VROPhysicsWorld> _physicsWorld = nullptr;
    
    void getBackgrounds(std::shared_ptr<VRONode> node, std::vector<std::shared_ptr<VROGeometry>> &backgrounds);
};

#endif /* VROScene_h */
