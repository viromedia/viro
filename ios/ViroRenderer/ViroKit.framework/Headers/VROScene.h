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
#include "VROTree.h"
#include "VROParticleEmitter.h"

class VRONode;
class VROPortal;
class VRORenderContext;
class VRODriver;
class VROTexture;
class VROGeometry;
class VROHitTestResult;
class VROVector3f;
class VROVector4f;
class VROAudioPlayer;
class VRORenderMetadata;
class VROInputControllerBase;

class VROScene : public std::enable_shared_from_this<VROScene>, public VROThreadRestricted {
    
public:
    
    VROScene();
    virtual ~VROScene();
    
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
    void updateSortKeys(std::shared_ptr<VRORenderMetadata> &metadata,
                        const VRORenderContext &context,
                        std::shared_ptr<VRODriver> &driver);
        
    /*
     Retrieve the root node of the scene.
     */
    std::shared_ptr<VROPortal> getRootNode();
    
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
     Particle Emitters are stored and computed per scene.
     */
    void computeParticles(const VRORenderContext &context) {
        for (std::shared_ptr<VROParticleEmitter> emitter: _activeParticles) {
            emitter->update(context);
        }
    }

    void addParticleEmitter(std::shared_ptr<VROParticleEmitter> emitter) {
        _activeParticles.push_back(emitter);
    }

    void removeParticleEmitter(std::shared_ptr<VROParticleEmitter> emitter) {
        _activeParticles.erase(std::remove(_activeParticles.begin(), _activeParticles.end(), emitter), _activeParticles.end());
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
     Set the 'active', or present portal. This is the portal the user is
     in, and determines the render-order of the stenciling, which in turn
     creates the illusion of being in the specified portal.
     */
    void setActivePortal(std::shared_ptr<VROPortal> node);
    
    /*
     Get all backgrounds in the scene.
     */
    std::vector<std::shared_ptr<VROGeometry>> getBackgrounds() const;
    
    /*
     Get the portal tree. Reconstructed each frame.
     */
    const tree<std::shared_ptr<VROPortal>> getPortalTree() const;
    
    /*
     Get all the lights in the scene, as collected from the last render cycle.
     */
    const std::vector<std::shared_ptr<VROLight>> &getLights() const {
        return _lights;
    }
    
    const std::shared_ptr<VROPortal> getActivePortal() const {
        return _activePortal;
    }

    /*
     Sets a list of post processing effects corresponding to VROPostProcessEffect to be applied
     on this scene.
     */
    void setSceneEffect(std::vector<std::string> effects){
        _activeEffects = effects;
        _shouldResetEffects = true;
    }

    /*
     Called by the renderer to process the latest list of post processing effects to be applied
     on this scene. Returns false if the list has already been processed and is thus an old list.
     Else returns true otherwise to indicate that a new effect list has been set and scheduled to
     be applied by the renderer.
     */
    bool processSceneEffect(std::vector<std::string> &effects){
        effects = _activeEffects;

        bool hasNewEffects = _shouldResetEffects;
        _shouldResetEffects = false;
        return hasNewEffects;
    }

    void setShouldResetEffects(bool reset) {
        _shouldResetEffects = reset;
    }

protected:
    
    /*
     The root node of the scene.
     */
    std::shared_ptr<VROPortal> _rootNode;

    /*
     UI representation of the underlying controller
     */
    std::shared_ptr<VROInputPresenter> _controllerPresenter;
    
    /*
     The portals in tree form, with the active portal at the root.
     */
    tree<std::shared_ptr<VROPortal>> _portals;
    
    /*
     All the lights in the scene, as collected during the last render cycle.
     */
    std::vector<std::shared_ptr<VROLight>> _lights;
    
    /*
     The distance from the camera of the furthest away object, since the last
     call to updateSortKeys. Distance is from the camera to the bounding
     box of the object.
     */
    float _distanceOfFurthestObjectFromCamera;

    /*
     Manages the physics in the scene.
     */
    std::shared_ptr<VROPhysicsWorld> _physicsWorld = nullptr;

    /*
     Represents a list of all actively emitting particles in this scene.
     */
    std::vector<std::shared_ptr<VROParticleEmitter>> _activeParticles;
    
    /*
     The active portal; the scene is rendered as though the camera is in this
     portal. Defaults to the root node.
     */
    std::shared_ptr<VROPortal> _activePortal;
    
    /*
     Create a tree of portals in the scene graph, with the active portal at the
     root of the tree. Note we use graph traversal because portal stencil rendering
     begins at the 'active' node, not necesarily at the root node.
     */
    void createPortalTree(const VRORenderContext &context);
    
    /*
     Helper function, sorts portals at each recursion level by distance from camera.
     */
    void sortSiblingPortals(tree<std::shared_ptr<VROPortal>> &tree, const VRORenderContext &context);
    
    /*
     Returns true if the given node is present in this scene.
     */
    bool hasNode(std::shared_ptr<VRONode> node) const;
    bool hasNode_helper(const std::shared_ptr<VRONode> &candidate, const std::shared_ptr<VRONode> &node) const;
    
    /*
     Retrieve all background textures in the scene.
     */
    void getBackgrounds(std::shared_ptr<VRONode> node, std::vector<std::shared_ptr<VROGeometry>> &backgrounds) const;
private:
    
    /*
     A helper method that draws a line from the max to min points of the given node's bounding box. Call
     from the render function if you want to see the lines
     */
    void drawBoundingBoxCorners(std::shared_ptr<VRONode> node,
                                const VRORenderContext &context,
                                std::shared_ptr<VRODriver> &driver);

    /*
     List of post processing effects to be applied in the renderer.
     */
    std::vector<std::string> _activeEffects;

    /*
     True if we should reset effects
     */
    bool _shouldResetEffects = false;
};

#endif /* VROScene_h */
