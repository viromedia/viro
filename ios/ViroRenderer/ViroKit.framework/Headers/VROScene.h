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
     Render the background of the scene.
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
     Add a new root node to the scene.
     */
    void addNode(std::shared_ptr<VRONode> node);
    std::vector<std::shared_ptr<VRONode>> &getRootNodes() {
        return _nodes;
    }
    
    /*
     Returns the associated physics world with this scene. If there's none
     one is created and returned.
     */
    std::shared_ptr<VROPhysicsWorld> getPhysicsWorld(){
        if (_physicsWorld == nullptr){
            _physicsWorld = std::make_shared<VROPhysicsWorld>();
        }
        return _physicsWorld;
    }

    /*
     Grabs and computes a physics world if one exists.
     */
    void computePhysics(){
        if (_physicsWorld != nullptr){
            _physicsWorld->computePhysics();
        }
    }

    /*
     Set the background of the scene to a cube-map defined by
     the given cube texture or color.
     */
    void setBackgroundCube(std::shared_ptr<VROTexture> textureCube);
    void setBackgroundCube(VROVector4f color);
    
    /*
     Set the background of the scene to textured sphere. Its rotation
     about the Y axis can be set as well.
     */
    void setBackgroundSphere(std::shared_ptr<VROTexture> textureSphere);
    void setBackgroundRotation(VROQuaternion rotation);
    
    /*
     Set the background of the scene to an arbitrary geometry. All this
     guarantees is that the given object will be rendered first. No
     properties will be set on this geometry, but typically background
     geometries are screen-space, and do not read or write to teh depth
     buffer.
     */
    void setBackground(std::shared_ptr<VROGeometry> geometry);
    
    std::shared_ptr<VROGeometry> getBackground() const {
        return _background;
    }
    void attachInputController(std::shared_ptr<VROInputControllerBase> controller);
    void detachInputController(std::shared_ptr<VROInputControllerBase> controller);
    std::shared_ptr<VROInputPresenter> getControllerPresenter();
    
    float getDistanceOfFurthestObjectFromCamera() const {
        return _distanceOfFurthestObjectFromCamera;
    }

private:
    
    /*
     The root nodes of the scene.
     */
    std::vector<std::shared_ptr<VRONode>> _nodes;
    
    /*
     The background visual to display. Rendered before any nodes.
     */
    std::shared_ptr<VROGeometry> _background;

    /*
     UI representation of the underlying controller
     */
    std::shared_ptr<VROInputPresenter> _controllerPresenter;
    
    /*
     The rotation to apply to the background geometry.
     */
    VROQuaternion _backgroundRotation;
    
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
};

#endif /* VROScene_h */
