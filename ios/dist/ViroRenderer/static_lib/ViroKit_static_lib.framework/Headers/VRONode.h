//
//  VRONode.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/15/15.
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

#ifndef VRONode_h
#define VRONode_h

#include <memory>
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <functional>
#include "optional.hpp"
#include "VROAtomic.h"
#include "VROMatrix4f.h"
#include "VROQuaternion.h"
#include "VRORenderContext.h"
#include "VRODriver.h"
#include "VRORenderParameters.h"
#include "VROAnimatable.h"
#include "VROBoundingBox.h"
#include "VROSortKey.h"
#include "VROLog.h"
#include "VROEventDelegate.h"
#include "VROSound.h"
#include "VROFrustumBoxIntersectionMetadata.h"
#include "VROThreadRestricted.h"
#include "VROPhysicsBody.h"

class VROGeometry;
class VROLight;
class VROScene;
class VROTaskQueue;
class VROAction;
class VROTexture;
class VROPortal;
class VROMorpher;
class VRONodeCamera;
class VROHitTestResult;
class VROConstraint;
class VROExecutableAnimation;
class VROTransformDelegate;
class VROTransaction;
class VRORenderMetadata;
class VROParticleEmitter;
class VROSkeletalAnimationLayer;
class VROSkinner;
class VROIKRig;

extern bool kDebugSortOrder;
extern int  kDebugSortOrderFrameFrequency;
extern const std::string kDefaultNodeTag;

enum class VRONodeType {
    Normal,
    Portal,
    PortalFrame,
};

enum class VROSilhouetteMode {
    Flat,             // Render silhouettes with constant lighting, no textures
    Textured,         // Render silhouettes with constant lighting and textures
};

enum class VRODragType {
    // Drags objects with a fixed distance to camera/controller/etc, from the point at which
    // the user has grabbed the geometry containing this draggable node.
    FixedDistance,

    // Drags objects with a fixed distance to camera/controller/etc, from the point of this
    // node's position in world space.
    FixedDistanceOrigin,

    // Drags object along a given plane (point + normal)
    FixedToPlane,

    // Currently available to AR only. Attempts to drag object w.r.t. the real world.
    FixedToWorld,
};

class VRONode : public VROAnimatable, public VROThreadRestricted {
    
public:
    
    static void resetDebugSortIndex();
    
#pragma mark - Initialization
    
    /*
     Default constructor.
     */
    VRONode();
    
    /*
     Copy constructor. This copies the node but *not* the underlying
     geometries or lights. Instead, these are shared by reference with the
     copied node. Additionally, this constructor will not copy child nodes.
     
     To copy child nodes recursively, invoke the clone() function.
     */
    VRONode(const VRONode &node);
    virtual ~VRONode();
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     
     Recurses down the tree.
     */
    virtual void deleteGL();
    
    /*
     Copy constructor that recursively copies all child nodes. This copies
     the node but *not* the underlying geometries or lights. Instead, these
     are shared by reference with the copied node.
     */
    std::shared_ptr<VRONode> clone();

    /*
     Get a unique ID for this VRONode.
     */
    int getUniqueID() { return _uniqueID; }
    
#pragma mark - Render Cycle

    /*
     Recursive function that recomputes the transforms of this node. This includes:
     
     _worldTransform,
     _worldRotation,
     _worldPosition,
     _worldBoundingBox
     */
    void computeTransforms(VROMatrix4f parentTransform, VROMatrix4f parentRotation);

    /*
     Sets both the local position and rotation of this node in terms of world coordinates.
     A computeTransform pass is then performed to update the node's bounding boxes
     and as well as its child's node transforms recursively. The animated flag should be
     false in most cases because of the recursive computeTransform pass.
     */
    void setWorldTransform(VROVector3f finalPosition, VROQuaternion finalRotation, bool animated = false);

    /*
     Update the visibility status of this node, using the camera in the current render
     context. This will update the _visible flag. Recurses to children.
     */
    void updateVisibility(const VRORenderContext &context);
    
    /*
     Update the particle emitters attached to this node. Recurses to children.
     */
    void updateParticles(const VRORenderContext &context);
    
    /*
     Recursively applies transformation constraints (e.g. billboarding) to this node
     and its children.
     */
    void applyConstraints(const VRORenderContext &context, VROMatrix4f parentTransform,
                          bool parentUpdated);
    
    /*
     Update the position of each light in this node, and add to the outLights vector.
     Recurses down the tree.
     */
    void collectLights(std::vector<std::shared_ptr<VROLight>> *outLights);
    
    /*
     Recursively updates the sort keys of this node, preparing this node and its children
     for rendering. This method also computes non-transform-related parameters for each
     node (opacity, lights, etc.) that are required prior to render, and outputs metadata
     about the forthcoming render to VRORenderMetadata.
     
     Note: this method and getSortKeys() *only* apply to visible nodes. Invisible nodes
     are skipped.
     */
    void updateSortKeys(uint32_t depth,
                        VRORenderParameters &params,
                        std::shared_ptr<VRORenderMetadata> &metadata,
                        const VRORenderContext &context,
                        std::shared_ptr<VRODriver> &driver);
    
    /*
     Get the sort keys for all visible nodes in this portal. Stops the search
     when we reach the hit of the scene graph or hit another portal.
     */
    void getSortKeysForVisibleNodes(std::vector<VROSortKey> *outKeys);
    
    /*
     Render the given element of this node's geometry, using its latest computed transforms.
     */
    void render(int elementIndex,
                std::shared_ptr<VROMaterial> &material,
                const VRORenderContext &context,
                std::shared_ptr<VRODriver> &driver);
    
    /*
     Recursively render this node and all of its children, with full texture
     and lighting.
     
     Note: this method does not intelligently batch or sort. It is is less efficient
     than directly calling render(..) above after proper sorting.
     */
    void render(const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
    /*
     Recursively render this the silhouette of this node and all of its children.
     Lighting is fixed at constant, and the given material is used for all elements.
     If mode is set to Textured, then textures will be bound. This method is typically
     used to render to the stencil or depth buffers only.
     
     The filter is used to only render the silhouettes of specific objects. Returns
     true on each node to render, false to not. Either way we continue down the tree
     recursively.
     */
    void renderSilhouettes(std::shared_ptr<VROMaterial> &material, VROSilhouetteMode mode,
                           std::function<bool(const VRONode&)> filter,
                           const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
    /*
     This function recomputes this node's transform before recomputing its umbrella bounding box
     using its parent's last computed transform.
     */
    void recomputeUmbrellaBoundingBox();
    
#pragma mark - Geometry
    
    void setGeometry(std::shared_ptr<VROGeometry> geometry) {
        passert_thread(__func__);
        _geometry = geometry;
    }
    std::shared_ptr<VROGeometry> getGeometry() const {
        return _geometry;
    }

    void setIKRig(std::shared_ptr<VROIKRig> rig) {
        _IKRig = rig;
    }
    std::shared_ptr<VROIKRig> getIKRig() {
        return _IKRig;
    }

    /*
     Called during a render pass to perform a full IK calculation on the rig
     attached to this node.
     */
    void computeIKRig();

#pragma mark - Camera
    
    void setCamera(std::shared_ptr<VRONodeCamera> camera) {
        passert_thread(__func__);
        _camera = camera;
    }
    const std::shared_ptr<VRONodeCamera> &getCamera() const {
        return _camera;
    }
    
#pragma mark - Transforms
    
    VROVector3f getWorldPosition() const;
    VROMatrix4f getWorldRotation() const;
    VROMatrix4f getWorldTransform() const;

    VROVector3f getPosition() const {
        return _position;
    }
    VROVector3f getScale() const {
        return _scale;
    }
    VROQuaternion getRotation() const {
        return _rotation;
    }
    VROVector3f getRotationEuler() const {
        return _euler;
    }

    /*
     The following are atomic, updated once per frame on the rendering thread. They can
     be accessed safely from any thread to get an up-to-date state of the transform.
     */
    VROMatrix4f    getLastWorldTransform() const;
    VROVector3f    getLastWorldPosition() const;
    VROMatrix4f    getLastWorldRotation() const;
    VROVector3f    getLastLocalPosition() const;
    VROVector3f    getLastLocalScale() const;
    VROQuaternion  getLastLocalRotation() const;
    VROMatrix4f    getLastScalePivot() const;
    VROMatrix4f    getLastRotationPivot() const;
    VROBoundingBox getLastWorldUmbrellaBoundingBox() const;
    VROBoundingBox getLastLocalUmbrellaBoundingBox() const;
    VROBoundingBox getLastLocalBoundingBox() const;
    
    /*
     The atomic geometry bounding box is set on the application thread as soon as a
     geometry is set for this Node.
     */
    void setLastGeometryBoundingBox(VROBoundingBox bounds);
    
    /*
     Set the rotation, position, or scale. Animatable.
     */
    void setRotation(VROQuaternion rotation);
    void setPosition(VROVector3f position);
    void setScale(VROVector3f scale);
    void setTransformDelegate(std::shared_ptr<VROTransformDelegate> delegate);
    
    /*
     Set the rotation as a vector of Euler angles. Using this method
     will update the Euler angles stored internally in a predictable
     way. Setting rotation by quaternion updates Euler angles in an 
     unpredictable way (i.e. the quaternion axis may change).
     */
    void setRotationEuler(VROVector3f euler);
    
    /*
     These piecewise setters are used in order to change one axis
     only, without altering the remaining axes. Useful when animating
     across multiple axes across separate calls. Animatable.
     */
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    void setRotationEulerX(float radians);
    void setRotationEulerY(float radians);
    void setRotationEulerZ(float radians);
    
    /*
     Pivot points define the center for rotation and scale. For example, 
     by translating the rotation pivot, you can use rotation to rotate an
     object about a faraway point. By translating the scale pivot, you can 
     scale an object relative to its corner, instead of its center. Not
     animatable.
     */
    void setRotationPivot(VROMatrix4f pivot);
    void setScalePivot(VROMatrix4f pivot);
    
    /*
     Get the bounding box and the umbrella bounding box. The former is the bounding box
     of just this node and its geometry; the latter is the union of this node's bounding
     box and that of all of its child nodes, descending recurisvely down the scene graph.
     */
    VROBoundingBox getBoundingBox() const;
    VROBoundingBox getUmbrellaBoundingBox() const;
    
#pragma mark - Application Thread Properties

    // Viro platforms (e.g. ViroCore) in general set properties on the main thread and dispatch those setters
    // to the rendering thread. This maintains thread-safety (and speed) because we don't
    // interfere with the ongoing render cycle when setting variables. However, it's common that
    // the user wants to set something on the application thread and then immediately invoke some
    // computation utilizing said variable, before it's been synchronized with the rendering
    // thread. For this reason we copy all relevant fields from VRONode into std::atomic variables.
    // These variables can be accessed from any thread. This way, we:
    //
    // 1. Maintain speed on the rendering thread (e.g. we don't have to lock or deal with atomics)
    // 2. Maintain access of this data across threads
    //
    // In other words, these atomic fields are *duplicates* of rendering thread fields, but are
    // accessible from the application thread. They are set in two ways:
    //
    // 1. By the application, via any atomic setter, from the application thread.
    // 2. By the renderer, via automatic sync with the rendering thread counterparts,
    //    once per frame. This mode of update is required because the renderer itself
    //    changes these variables through internal processes like physics and animation.

    // The atomic setters below will immediately update all of a node's related application
    // thread properties. For example, node->setPositionAtomic() will immediately update the
    // application thread's world transform, so that it can be used for other calculations on
    // the application thread. These setters will *dispatch* to the rendering thread to set the
    // corresponding rendering thread properties.
    void setPositionAtomic(VROVector3f position);
    void setRotationAtomic(VROQuaternion rotation);
    void setScaleAtomic(VROVector3f scale);
    void setScalePivotAtomic(VROMatrix4f scalePivot);
    void setRotationPivotAtomic(VROMatrix4f rotationPivot);
    
    /*
     Must be invoked for this node and its children (all the way down the scene graph) whenever
     atomic position, scale, scale pivot, rotation, or rotation pivot are set. Computes _lastWorldTransform,
     _lastWorldPosition, _lastWorldRotation, and _lastWorldBoundingBox, on this node only. Requires the
     latest data from this node's parent to make the computations.

     This does not recurse down the scene graph on its own because we do not have access to an
     application thread copy of the scene graph. ViroCore does have such a copy in Java-land, so it
     handles the recursive invocation of this method.
     */
    void computeTransformsAtomic(VROMatrix4f parentTransform, VROMatrix4f parentRotation);
    
    /*
     Helper functions used to update the _lastWorldUmbrellaBoundingBox of the given node with the world
     bounds of _this_ node; that is, the bounds of this node will be union-ed with the bounds of the
     given node. Return the transform needed for the next recursion.
     */
    void startComputeAtomicUmbrellaBounds();
    VROMatrix4f computeAtomicUmbrellaBounds(std::shared_ptr<VRONode> parentNodeBeingUpdated, VROMatrix4f transform);
    void endComputeAtomicUmbrellaBounds();
    
    /*
     Recursively sync the application thread properties with the latest values from the rendering
     thread. Called on the rendering thread after the transform computation occurs in the render
     cycle. Dispatches to the application thread.
     */
    void syncAppThreadProperties();
    
#pragma mark - Render Settings
    
    std::string getName() const {
        return _name;
    }
    void setName(std::string name) {
        _name = name;
    }
    
    float getOpacity() const {
        return _opacity;
    }
    void setOpacity(float opacity);

    virtual bool isHidden() const {
        return _hidden;
    }
    virtual void setHidden(bool hidden);
    
    int getRenderingOrder() const {
        return _renderingOrder;
    }
    void setRenderingOrder(int renderingOrder) {
        _renderingOrder = renderingOrder;
    }
    
    bool isHierarchicalRendering() const {
        return _hierarchicalRendering;
    }
    void setHierarchicalRendering(bool hierarchicalRendering) {
        _hierarchicalRendering = hierarchicalRendering;
    }
    
    /*
     True to stop rendering of this node and all of its children until the
     model load callbacks are finished. Used internally.
     */
    void setHoldRendering(bool hold) {
        _holdRendering = hold;
    }
    
    /*
     Returns true if this node was found visible during the last call to
     computeVisibility(). If a node is not visible, that means none of its
     children are visible either (we use the umbrella bounding box for
     visibility tests).
     */
    bool isVisible() const {
        return _visible;
    }
    
    /*
     Debug function to count the number of visible nodes (including this
     node if visible, then recursively descending from this node's children)
     since the last call to computeVisibility().
     */
    int countVisibleNodes() const;

#pragma mark - Particle Emitters

    void setParticleEmitter(std::shared_ptr<VROParticleEmitter> emitter);
    void removeParticleEmitter();
    std::shared_ptr<VROParticleEmitter> getParticleEmitter() const;

#pragma mark - Lights
    
    void addLight(std::shared_ptr<VROLight> light) {
        passert_thread(__func__);
        _lights.push_back(light);
    }
    void removeLight(std::shared_ptr<VROLight> light) {
        passert_thread(__func__);
        _lights.erase(
                      std::remove_if(_lights.begin(), _lights.end(),
                                     [light](std::shared_ptr<VROLight> candidate) {
                                         return candidate == light;
                                     }), _lights.end());
    }
    void removeAllLights() {
        passert_thread(__func__);
        _lights.clear();
    }
    std::vector<std::shared_ptr<VROLight>> &getLights() {
        return _lights;
    }
    const std::vector<std::shared_ptr<VROLight>> &getComputedLights() const {
        return _computedLights;
    }
    uint32_t getComputedLightsHash() const {
        return _computedLightsHash;
    }
    
    void setLightReceivingBitMask(int bitMask, bool recursive = false) {
        _lightReceivingBitMask = bitMask;
        if (recursive) {
            for (std::shared_ptr<VRONode> &child : _subnodes) {
                child->setLightReceivingBitMask(bitMask, recursive);
            }
        }
    }
    int getLightReceivingBitMask() const {
        return _lightReceivingBitMask;
    }
    
    void setShadowCastingBitMask(int bitMask, bool recursive = false) {
        _shadowCastingBitMask = bitMask;
        if (recursive) {
            for (std::shared_ptr<VRONode> &child : _subnodes) {
                child->setShadowCastingBitMask(bitMask, recursive);
            }
        }
    }
    int getShadowCastingBitMask() const {
        return _shadowCastingBitMask;
    }

#pragma mark - Sounds
    
    void addSound(std::shared_ptr<VROSound> sound) {
        passert_thread(__func__);
        if (sound->getType() == VROSoundType::Spatial) {
            _sounds.push_back(sound);
        }
    }
    void removeSound(std::shared_ptr<VROSound> sound) {
        passert_thread(__func__);
        _sounds.erase(
                std::remove_if(_sounds.begin(), _sounds.end(),
                               [sound](std::shared_ptr<VROSound> candidate) {
                                   return candidate == sound;
                               }), _sounds.end());
    }
    void removeAllSounds() {
        passert_thread(__func__);
        _sounds.clear();
    }

#pragma mark - Scene Graph
    
    void addChildNode(std::shared_ptr<VRONode> node);
    void removeFromParentNode();
    
    /*
     Return a copy of the subnode list.
     */
    std::vector<std::shared_ptr<VRONode>> getChildNodes() const;
    
    /*
     Remove all children from this node.
     */
    void removeAllChildren();
    
    /*
     Return the parent node. Null if this node is root or does not have a parent.
     */
    std::shared_ptr<VRONode> getParentNode() const {
        return _supernode.lock();
    }
    
    /*
     Get the parent scene of this VRONode. If this node is not attached to the
     scene graph, this will return null.
     */
    std::shared_ptr<VROScene> getScene() const {
        return _scene.lock();
    }

    /*
     Returns a vec of skinners associated with this node. If recurse is true, we also
     examine recursively down the subtree and return any found skinners as well.
     */
    void getSkinner(std::vector<std::shared_ptr<VROSkinner>> &skinnerOut, bool recurse);

    /*
     Set the parent scene of this node. Internal use only.
     */
    void setScene(std::shared_ptr<VROScene> scene, bool recursive);
    
    /*
     Returns the type of this node. Faster then dynamic_cast.
     */
    VRONodeType getType() const {
        return _type;
    }
    
    /*
     Get the nearest portal that's an ancestor of this node. Returns null if this is
     the root node.
     */
    const std::shared_ptr<VROPortal> getParentPortal() const;
    
    /*
     Get the nearest child portals of this node. This recurses down the graph in all
     directions, stopping whenever we hit a portal or the end of the graph.
     */
    void getChildPortals(std::vector<std::shared_ptr<VROPortal>> *outPortals) const;
    
#pragma mark - Actions and Animations
    
    /*
     Actions enable open-ended and fully customizable manipulation of nodes over successive
     frames.
     */
    void runAction(std::shared_ptr<VROAction> action);
    void removeAction(std::shared_ptr<VROAction> action);
    void removeAllActions();
    
    /*
     Animations enable structured manipulation of nodes over successive frames. They can
     be as simple interpolating batches of properties over time, or as complex as full
     skeletal animation.
     
     These methods take a key parameter. Keys identify animations that run together in 
     a single transaction; e.g., there can be multiple animations with a single key.
     removeAnimation will remove *all* animations with the given key.
     */
    void addAnimation(std::string key, std::shared_ptr<VROExecutableAnimation> animation);
    void removeAnimation(std::string key);
    
    /*
     Get the keys for all animations in this node. If recursive is true, will search 
     down the hierarchy as well.
     */
    std::set<std::string> getAnimationKeys(bool recursive);
    
    /*
     Retrieve all animations with the given key, as a single, composite executable animation. 
     If recursive is true, then this will return a new parallel VROAnimationChain that 
     contains every animation in this node and every animation in any subnode that shares
     the same key.
     
     For example, if the animation 'Take 001' contains a torso animation and an arm animation,
     both will be returned in a single animation group.
     */
    std::shared_ptr<VROExecutableAnimation> getAnimation(std::string key, bool recursive);
    
    /*
     Retrieve all the animations with the given keys as a single, composite executable
     animation. If multiple animations influence the same bone, the provided weights determine
     how the animations blend.
     
     If recursive is true, this will search subnodes for animations as well.
     
     For example, if the animation 'Body' and the animation 'LeftArm' contain torso and left
     arm animations, both will be returned in a single animation group. If both animations
     move the left arm, their influences on the left arm will be blended.
     */
    std::shared_ptr<VROExecutableAnimation> getLayeredAnimation(std::vector<std::shared_ptr<VROSkeletalAnimationLayer>> layers,
                                                                bool recursive);
    
    /*
     Remove all animations from this node.
     */
    void removeAllAnimations();

    /*
     Triggered when the animation running this animatable node completes.
     */
    void onAnimationFinished();

    /*
     Returns a set of VROMorphers containing all morph targets that are associated with this node.
     If recursive is true, we will search down the node hierarchy as well.
     */
    std::set<std::shared_ptr<VROMorpher>> getMorphers(bool recursive);

#pragma mark - Events
    
    std::vector<VROHitTestResult> hitTest(const VROCamera &camera, VROVector3f origin, VROVector3f ray,
                                          bool boundsOnly = false);
    
    void setSelectable(bool selectable) {
        _selectable = selectable;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delegate) {
        passert_thread(__func__);
        _eventDelegateWeak = delegate;
    }

    std::shared_ptr<VROEventDelegate> getEventDelegate() {
        if (_eventDelegateWeak.expired()){
            return nullptr;
        }
        return _eventDelegateWeak.lock();
    }

    bool isSelectable() const {
        return _selectable;
    }

    void setIgnoreEventHandling(bool canHandle) {
        _ignoreEventHandling = canHandle;

        for (std::shared_ptr<VRONode> childNode : getChildNodes()){
            childNode->setIgnoreEventHandling(canHandle);
        }
    }

    bool getIgnoreEventHandling() const {
        return _ignoreEventHandling;
    }

    void setTag(std::string tag) {
        _tag = tag;
    }
    std::string getTag() const {
        return _tag;
    }
    
    void setHighAccuracyEvents(bool enabled);
    bool getHighAccuracyEvents() const {
        return _highAccuracyEvents;
    }

    void setIsBeingDragged(bool isDragging) {
        std::shared_ptr<VROPhysicsBody> physicsBody = getPhysicsBody();
        if (physicsBody != nullptr) {
            physicsBody->setKinematicDrag(isDragging);
        }
    }

    void setDragType(VRODragType dragType) {
        _dragType = dragType;
    }
    VRODragType getDragType() {
        return _dragType;
    }

    void setDragPlanePoint(VROVector3f point) {
        _dragPlanePoint = point;
    }
    VROVector3f getDragPlanePoint() {
        return _dragPlanePoint;
    }

    void setDragPlaneNormal(VROVector3f normal) {
        _dragPlaneNormal = normal;
    }
    VROVector3f getDragPlaneNormal() {
        return _dragPlaneNormal;
    }

    void setDragMaxDistance(float maxDistance) {
        _dragMaxDistance = maxDistance;
    }
    float getDragMaxDistance() {
        return _dragMaxDistance;
    }

    bool isAnimatingDrag() {
        return _isAnimatingDrag;
    }
    void setIsAnimatingDrag(bool isAnimatingDrag) {
        _isAnimatingDrag = isAnimatingDrag;
    }

    std::shared_ptr<VROTransaction> getDragAnimation() {
        return _dragAnimation;
    }
    void setDragAnimation(std::shared_ptr<VROTransaction> dragAnimation) {
        _dragAnimation = dragAnimation;
    }

#pragma mark - Constraints
    
    void addConstraint(std::shared_ptr<VROConstraint> constraint);
    void removeConstraint(std::shared_ptr<VROConstraint> constraint);
    void removeAllConstraints();

#pragma mark - Physics
    
    std::shared_ptr<VROPhysicsBody> initPhysicsBody(VROPhysicsBody::VROPhysicsBodyType type,
                                                    float mass,
                                                    std::shared_ptr<VROPhysicsShape> shape);
    std::shared_ptr<VROPhysicsBody> getPhysicsBody() const;
    void clearPhysicsBody();
    
#pragma mark - Task Queues
    
    void addTaskQueue(std::shared_ptr<VROTaskQueue> queue);
    void removeTaskQueue(std::shared_ptr<VROTaskQueue> queue);

protected:
    
    VRONodeType _type;
    
    /*
     The node's parent and children.
     */
    std::vector<std::shared_ptr<VRONode>> _subnodes;
    std::weak_ptr<VRONode> _supernode;
    
    /*
     The VROScene to which this node belongs.
     */
    std::weak_ptr<VROScene> _scene;
    
    /*
     The geometry in the node. Null means the node has no geometry.
     */
    std::shared_ptr<VROGeometry> _geometry;

    /*
     The inverse kinematic rig associated with this node, set when this node is
     considered the root node joint of the rig.
     */
    std::shared_ptr<VROIKRig> _IKRig;

    /*
     True if this node was found visible during the last call to computeVisibility().
     */
    bool _visible;
    
    /*
     Last frame that this node was visited during sorting. Used for graph traversal.
     */
    int _lastVisitedRenderingFrame;

private:
    
    /*
     Name for debugging.
     */
    std::string _name;

    /*
     Unique identifier.
     */
    int _uniqueID;
    
    /*
     Lights, sound, particles, and camera.
     */
    std::vector<std::shared_ptr<VROLight>> _lights;
    std::vector<std::shared_ptr<VROSound>> _sounds;
    std::shared_ptr<VROParticleEmitter> _particleEmitter;
    std::shared_ptr<VRONodeCamera> _camera;
    
    /*
     Scale and position.
     */
    VROVector3f _scale;
    VROVector3f _position;
    
    /*
     Rotation is stored as a quaternion, but we also maintain euler angles
     for use in animation (since we cannot additively rotate by reading euler
     angles from a quaternion and writing them again).
     */
    VROQuaternion _rotation;
    VROVector3f _euler;
    
    /*
     Pivots define the center of the rotation and scale operations. 
     Declared optional becuase they are not always used, and we can optimize 
     them away when not used.
     */
    std::experimental::optional<VROMatrix4f> _rotationPivot;
    std::experimental::optional<VROMatrix4f> _rotationPivotInverse;
    std::experimental::optional<VROMatrix4f> _scalePivot;
    std::experimental::optional<VROMatrix4f> _scalePivotInverse;
    
    /*
     User-defined rendering order for this node.
     */
    int _renderingOrder;
    
    /*
     Parameters computed by descending down the tree. These are updated whenever
     any parent or this node itself is updated. For example, computedOpacity is
     the opacity of this node multiplied by the opacities of all this node's
     ancestors. Similarly, worldTransform is the full cascaded transformation
     matrix for the node. 
     
     worldRotation only takes into account rotations (not scale or translation).
     computedLights are the lights that influence this node, based on distance from
     the light and light attenuation, unrelated to the scene graph (e.g. the lights
     in _computedLights may belong to any node in the scene).
     
     localTransform only takes into the account the transformations of _this_
     node.
     */
    VROMatrix4f _localTransform;
    VROMatrix4f _worldTransform;
    VROMatrix4f _worldInverseTransposeTransform;
    VROMatrix4f _worldRotation;
    VROVector3f _worldPosition;
    float _computedOpacity;
    std::vector<std::shared_ptr<VROLight>> _computedLights;
    uint32_t _computedLightsHash;
    std::weak_ptr<VROTransformDelegate> _transformDelegate;

    /*
     Application-thread copies of the node's transform data. See the 'Application Thread
     Properties' pragma above for a more extensive description of why we need these fields.
     The following are computed fields (not directly set by users).
     */
    VROAtomic<VROMatrix4f> _lastLocalTransform;
    VROAtomic<VROMatrix4f> _lastWorldTransform;
    VROAtomic<VROVector3f> _lastWorldPosition;
    VROAtomic<VROMatrix4f> _lastWorldRotation;
    
    /*
     Bounding boxes are either defined in world or local coordinates, and encapsulate
     either _this_ node only, or this node and all children (umbrella).
     */
    VROAtomic<VROBoundingBox> _lastWorldBoundingBox;
    VROAtomic<VROBoundingBox> _lastLocalBoundingBox;
    VROAtomic<VROBoundingBox> _lastGeometryBoundingBox;
    VROAtomic<VROBoundingBox> _lastLocalUmbrellaBoundingBox;
    VROAtomic<VROBoundingBox> _lastWorldUmbrellaBoundingBox;
    bool _lastUmbrellaBoundsSet;
    
    /*
     Directly-set application thread properties.
     */
    VROAtomic<VROVector3f> _lastPosition;
    VROAtomic<VROVector3f> _lastScale;
    VROAtomic<VROQuaternion> _lastRotation;
    VROAtomic<VROMatrix4f> _lastScalePivot, _lastScalePivotInverse;
    VROAtomic<VROMatrix4f> _lastRotationPivot, _lastRotationPivotInverse;
    VROAtomic<bool> _lastHasScalePivot;
    VROAtomic<bool> _lastHasRotationPivot;

    /*
     The bounding box containing this node's geometry, in both local and world
     coordinates. The umbrella variant encompasses not only this geometry, but the
     geometries of all this node's children. The geometry bounding box is the
     bounding box for the geometry without any transforms applied, while the local
     bounding box is the bounding box with local transforms applied.
     */
    VROBoundingBox _geometryBoundingBox; // No transforms
    VROBoundingBox _localBoundingBox;    // Local transforms
    VROBoundingBox _worldBoundingBox;    // World transforms
    VROBoundingBox _localUmbrellaBoundingBox;
    VROBoundingBox _worldUmbrellaBoundingBox;
    VROFrustumBoxIntersectionMetadata _umbrellaBoxMetadata;
    
    /*
     True if this node is hidden. Hidden nodes are not rendered, and do not 
     respond to tap events. Hiding a node within an animation results in a 
     fade-out animation. The _opacityFromHiddenFlag is the opacity as derived
     from _hidden: 0.0 if _hidden is true, 1.0 if _hidden is false, or somewhere
     in-between during animation.
     */
    bool _hidden;
    float _opacityFromHiddenFlag;
    
    /*
     The opacity of the node (0.0 is transparent, 1.0 is opaque). When opacity
     drops below a threshold value, the node is hidden. This opacity is set by
     the user.
     */
    float _opacity;
    
    /*
     True if this node is selectable by hit testing. Defaults to true.
     */
    bool _selectable;

    /*
     True if this node is set to ignore all events fired from VROBaseInputController.
     */
    bool _ignoreEventHandling;

    /*
     Delegate through which events are notified from the VROEventManager.
     */
    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    /*
     True if we want to perform more accurate event hit testing against this node's geometry
     rather than its bounding box.
     */
    bool _highAccuracyEvents;

    /*
     Active actions on this node.
     */
    std::vector<std::shared_ptr<VROAction>> _actions;
    
    /*
     Animations stored with this node.
     */
    std::map<std::string, std::vector<std::shared_ptr<VROExecutableAnimation>>> _animations;
    
    /*
     Constraints on the node, which can modify the node's transformation matrix.
     */
    std::vector<std::shared_ptr<VROConstraint>> _constraints;
    
    /*
     True indicates that this node's descendants (children, grand-children, and so on)
     should be rendered by order of their scene graph depth, with depth reading disabled.
     Useful when rendering 2D layouts like flexbox views, where the parent components and
     their children have depths so close together that reverting to the painter's algorithm
     instead of depth testing produces better results (minimal z-fighting). Defaults to
     false.
     */
    bool _hierarchicalRendering;

    /*
     The drag type to use for this VRONode.
     */
    VRODragType _dragType;

    /*
     The point in 3D space on the plane to "drag"
     */
    VROVector3f _dragPlanePoint;

    /*
     The normal of the plane to "drag" along
     */
    VROVector3f _dragPlaneNormal;

    /*
     The max distance from the controller the user is allowed to drag an item (threshold behavior
     depends on the _dragType selected).
     */
    float _dragMaxDistance;

    /*
     Whether or not a drag is still being animated (used only if _dragType == VRODragType::FixedToWorld
     */
    bool _isAnimatingDrag;

    /*
     The VROTransaction representing the animation from dragging while _dragType == VRODragType::FixedToWorld.
     */
    std::shared_ptr<VROTransaction> _dragAnimation;
    
#pragma mark - Private

    /*
     Notifies attached transform delegate, if any, that a position change had occurred.
     */
    void notifyTransformUpdate(bool forced);
    
    /*
     Recursively set the visibility of this node and all of its children to the 
     given value.
     */
    void setVisibilityRecursive(bool visible);
    
    /*
     Recursively expand the given bounding box by this node's _worldBoundingBox.
     */
    void computeUmbrellaBounds();
    bool computeUmbrellaBounds(VROBoundingBox *localBounds, VROBoundingBox *worldBounds, VROMatrix4f transform, bool isSet) const;
    
    /*
     Compute the transform for this node, taking into the account the parent's transform.
     Updates all related variables:
     
     _worldTransform
     _worldPosition
     _worldBoundingBox
     */
    void doComputeTransform(VROMatrix4f parentTransform);
    
    /*
     Action processing: execute all current actions and remove those that are
     expired.
     */
    void processActions();
    
    /*
     Get the animations in this node under the given key, and add them to the
     given vector.
     */
    void getAnimations(std::vector<std::shared_ptr<VROExecutableAnimation>> &animations,
                       std::string key, bool recursive);
    
    /*
     Get the keys of all animations in this node, and add them to the given set.
     */
    void getAnimationKeys(std::set<std::string> &animations, bool recursive);
    
    /*
     Hit test helper functions.
     */
    void hitTest(const VROCamera &camera, VROVector3f origin, VROVector3f ray,
                 bool boundsOnly, std::vector<VROHitTestResult> &results);
    bool hitTestGeometry(VROVector3f origin, VROVector3f ray,
                         VROMatrix4f transform, VROVector3f *intPt);
    
    /*
     The light and shadow bit masks. These are logically ANDed with each light's
     influence bit mask.
     
     If the result is non-zero for the light bit mask, then the light will illuminate
     the node. If the result is zero, then this node will be excluded from the light's
     illumination, including receipt of that light's shadows.
     
     If the AND result is non-zero for the shadow casting bit map, then the node
     will be cast shadows from the light (e.g. it will be rendered to that light's
     shadow map). If the result is zero, it will not cast shadows from said light.
     
     These both default to 1.
     */
    int _lightReceivingBitMask;
    int _shadowCastingBitMask;

    /*
     Physics rigid body that if defined, drives and sets the transformations of this node.
     */
    std::shared_ptr<VROPhysicsBody> _physicsBody;

    /*
     Non-unique tag identifier representing this node. Defaults to kDefaultNodeTag.
     */
    std::string _tag = kDefaultNodeTag;
    
    /*
     Used internally to hold the rendering of a node and all of its children until a model
     load callback has been invoked.
     */
    bool _holdRendering;
    
    /*
     Task queus used for loading objects into this VRONode. We store these here in order
     to scope them to the lifetime of the node for which they are performing loading
     tasks.
     */
    std::vector<std::shared_ptr<VROTaskQueue>> _taskQueues;
    
};

#endif /* VRONode_h */
