//
//  VROScene.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/19/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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
enum class VROToneMappingMethod;

class VROScene : public std::enable_shared_from_this<VROScene>, public VROThreadRestricted {
    
public:
    
    VROScene();
    virtual ~VROScene();
    
#pragma mark - Core Render Cycle
    
    /*
     Compute the transforms, recursively, for all nodes in this scene.
     */
    void computeTransforms();

    /*
     Update the visibility status of all nodes in the scene graph.
     */
    void updateVisibility(const VRORenderContext &context);
    
    /*
     Update the particle emitters in the scene graph.
     */
    virtual void updateParticles(const VRORenderContext &context);
    
    /*
     Apply transformation constraints (e.g. billboarding) to all nodes in
     the scene.
     */
    void applyConstraints(const VRORenderContext &context);

    /*
     Applies a rig constraint computation pass to all applicable sub nodes.
     */
    void computeIKRig(const VRORenderContext &context);

    /*
     Notifies the root node that the render properties have settled and
     that it should sync them now with their corresponding atomics.
     */
    void syncAtomicRenderProperties();
    
    /*
     Update the sort keys for all nodes in this scene.
     */
    void updateSortKeys(std::shared_ptr<VRORenderMetadata> &metadata,
                        const VRORenderContext &context,
                        std::shared_ptr<VRODriver> &driver);
    
#pragma mark - Scene Introspection
    
    /*
     Retrieve the root node of the scene.
     */
    std::shared_ptr<VROPortal> getRootNode();
    
    float getDistanceOfFurthestObjectFromCamera() const {
        return _distanceOfFurthestObjectFromCamera;
    }
    
    /*
     Get all backgrounds in the scene.
     */
    std::vector<std::shared_ptr<VROGeometry>> getBackgrounds() const;
    
    /*
     Get all the lights in the scene, as collected from the last render cycle.
     */
    const std::vector<std::shared_ptr<VROLight>> &getLights() const {
        return _lights;
    }
    
#pragma mark - Physics
    
    bool hasPhysicsWorld() const {
        return _physicsWorld != nullptr;
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
     Computes the physics simulation for the current frame, if a VROPhysicsWorld
     exists.
     */
    void computePhysics(const VRORenderContext &context) {
        if (_physicsWorld != nullptr) {
            _physicsWorld->computePhysics(context);
        }
    }
    
#pragma mark - Input

    /*
     Attach or detach input controllers.
     */
    void attachInputController(std::shared_ptr<VROInputControllerBase> controller);
    void detachInputController(std::shared_ptr<VROInputControllerBase> controller);
    std::shared_ptr<VROInputPresenter> getControllerPresenter();
    
#pragma mark - Portals
    
    /*
     Set the 'active', or present portal. This is the portal the user is
     in, and determines the render-order of the stenciling, which in turn
     creates the illusion of being in the specified portal.
     */
    void setActivePortal(std::shared_ptr<VROPortal> node);
    
    /*
     Get the portal tree. Reconstructed each frame.
     */
    const tree<std::shared_ptr<VROPortal>> getPortalTree() const;
    
    /*
     Get the active portal, which is the portal the user is currently "inside".
     */
    const std::shared_ptr<VROPortal> getActivePortal() const {
        return _activePortal;
    }
    
#pragma mark - Post-processing
    
    /*
     Tone mapping settings. Note that linear rendering is controlled by the
     driver's getColorRenderingMode() method, while this setting merely controls
     whether or not we're using tone-mapping.
     
     If tone mapping is disabled, all HDR colors are translated directly into the
     8-bit framebuffer, clamped at 1.0.
     */
    void setToneMappingEnabled(bool enabled);
    void setToneMappingMethod(VROToneMappingMethod method);
    void setToneMappingExposure(float exposure);
    void setToneMappingWhitePoint(float whitePoint);
    void setToneMappingUpdated(bool updated);
    
    bool isToneMappingEnabled() const { return _toneMappingEnabled; }
    VROToneMappingMethod getToneMappingMethod() const { return _toneMappingMethod; }
    float getToneMappingExposure()   const { return _toneMappingExposure; }
    float getToneMappingWhitePoint() const { return _toneMappingWhitePoint; }
    bool isToneMappingUpdated()      const { return _toneMappingUpdated; }

    /*
     Set or get the post-processing effects installed on the scene. Each string
     corresponds to a VROPostProcessEffect.
     */
    void setPostProcessingEffects(std::vector<std::string> effects);
    std::vector<std::string> getPostProcessingEffects() const;

    /*
     Indicates if the renderer needs to update its installed post-processing effects.
     */
    bool isPostProcessingEffectsUpdated() const;
    void setPostProcessingEffectsUpdated(bool updated);

protected:
    
    /*
     The root node of the scene.
     */
    std::shared_ptr<VROPortal> _rootNode;
    
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
     List of post-processing effects to be applied in the renderer.
     */
    std::vector<std::string> _activePostProcessingEffects;
    
    /*
     True if we post-processing effects were updated since the last time the
     renderer checked.
     */
    bool _postProcessingEffectsUpdated;
    
    /*
     Tone mapping parameters, and flag indicating if any have been updated.
     */
    bool _toneMappingEnabled;
    VROToneMappingMethod _toneMappingMethod;
    float _toneMappingExposure;
    float _toneMappingWhitePoint;
    bool _toneMappingUpdated;
    
    /*
     A helper method that draws a line from the max to min points of the given node's bounding box. Call
     from the render function if you want to see the lines
     */
    void drawBoundingBoxCorners(std::shared_ptr<VRONode> node,
                                const VRORenderContext &context,
                                std::shared_ptr<VRODriver> &driver);
};

#endif /* VROScene_h */
