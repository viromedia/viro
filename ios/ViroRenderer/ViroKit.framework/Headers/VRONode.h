//
//  VRONode.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRONode_h
#define VRONode_h

#include <memory>
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include "optional.hpp"
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
class VROAction;
class VROTexture;
class VROPortal;
class VRONodeCamera;
class VROHitTestResult;
class VROConstraint;
class VROExecutableAnimation;
class VROTransformDelegate;

extern bool kDebugSortOrder;
extern const std::string kDefaultNodeTag;

enum class VRONodeType {
    Normal,
    Portal,
    PortalFrame,
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
     Copy constructor that recursively copies all child nodes. This copies
     the node but *not* the underlying geometries or lights. Instead, these
     are shared by reference with the copied node.
     */
    std::shared_ptr<VRONode> clone();
    
#pragma mark - Render Cycle

    /*
     Recursive function that recomputes the transforms of this node. This includes:
     
     _computedTransform,
     _computedRotation,
     _computedPosition,
     _computedBoundingBox
     */
    void computeTransforms(VROMatrix4f parentTransform, VROMatrix4f parentRotation);

    /*
     Sets both the local position and rotation of this node in terms of world coordinates.
     A computeTransform pass is then performed to update the node's bounding boxes
     and as well as its child's node transforms recursively.
     */
    void setWorldTransform(VROVector3f finalPosition, VROQuaternion finalRotation);

    /*
     Update the visibility status of this node, using the camera in the current render
     context. This will update the _visible flag. Recurses to children.
     */
    void updateVisibility(const VRORenderContext &context);
    
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
     node (opacity, lights, etc.) that are required prior to render.
     
     Note: this method and getSortKeys() *only* apply to visible nodes. Invisible nodes
     are skipped.
     */
    void updateSortKeys(uint32_t depth,
                        VRORenderParameters &params,
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
     Texture and lighting are ignored, and the given material is used. This
     method is typically used to render to the stencil or depth buffers only.
     */
    void renderSilhouette(std::shared_ptr<VROMaterial> &material,
                          const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
#pragma mark - Geometry
    
    void setGeometry(std::shared_ptr<VROGeometry> geometry) {
        passert_thread();
        _geometry = geometry;
    }
    std::shared_ptr<VROGeometry> getGeometry() const {
        return _geometry;
    }
    
#pragma mark - Camera
    
    void setCamera(std::shared_ptr<VRONodeCamera> camera) {
        passert_thread();
        _camera = camera;
    }
    const std::shared_ptr<VRONodeCamera> &getCamera() const {
        return _camera;
    }
    
#pragma mark - Transforms
    
    VROVector3f getComputedPosition() const;
    VROMatrix4f getComputedRotation() const;
    VROMatrix4f getComputedTransform() const;

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
     Set the rotation, position, or scale. Animatable.
     */
    void setRotation(VROQuaternion rotation);
    void setPosition(VROVector3f position);
    void setScale(VROVector3f scale);
    void setTransformDelegate(std::shared_ptr<VROTransformDelegate> delegate);

    /*
     Notifies attached transform delegate, if any, that a position change had occurred.
     */
    void notifyTransformUpdate(bool forced);
    
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
     Returns true if this node was found visible during the last call to
     computeVisibility(). If a node is not visible, that means none of its
     children are visible either (we use the umbrella bounding box for
     visibility tests).
     */
    bool isVisibile() const {
        return _visible;
    }
    
    /*
     Debug function to count the number of visible nodes (including this
     node if visible, then recursively descending from this node's children)
     since the last call to computeVisibility().
     */
    int countVisibleNodes() const;
    
#pragma mark - Lights
    
    void addLight(std::shared_ptr<VROLight> light) {
        passert_thread();
        _lights.push_back(light);
    }
    void removeLight(std::shared_ptr<VROLight> light) {
        passert_thread();
        _lights.erase(
                      std::remove_if(_lights.begin(), _lights.end(),
                                     [light](std::shared_ptr<VROLight> candidate) {
                                         return candidate == light;
                                     }), _lights.end());
    }
    void removeAllLights() {
        passert_thread();
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

#pragma mark - Sounds
    
    void addSound(std::shared_ptr<VROSound> sound) {
        passert_thread();
        if (sound->getType() == VROSoundType::Spatial) {
            _sounds.push_back(sound);
        }
    }
    void removeSound(std::shared_ptr<VROSound> sound) {
        passert_thread();
        _sounds.erase(
                std::remove_if(_sounds.begin(), _sounds.end(),
                               [sound](std::shared_ptr<VROSound> candidate) {
                                   return candidate == sound;
                               }), _sounds.end());
    }

#pragma mark - Scene Graph
    
    void addChildNode(std::shared_ptr<VRONode> node) {
        passert_thread();
        passert (node);
        
        _subnodes.push_back(node);
        node->_supernode = std::static_pointer_cast<VRONode>(shared_from_this());
    }
    void removeFromParentNode() {
        passert_thread();
        
        std::shared_ptr<VRONode> supernode = _supernode.lock();
        if (supernode) {
            std::vector<std::shared_ptr<VRONode>> &parentSubnodes = supernode->_subnodes;
            parentSubnodes.erase(
                                 std::remove_if(parentSubnodes.begin(), parentSubnodes.end(),
                                                [this](std::shared_ptr<VRONode> node) {
                                                    return node.get() == this;
                                                }), parentSubnodes.end());
            _supernode.reset();
        }
    }
    
    /*
     Return a copy of the subnode list.
     */
    std::vector<std::shared_ptr<VRONode>> getChildNodes() const {
        return _subnodes;
    }
    
    /*
     Return the parent node. Null if this node is root or does not have a parent.
     */
    std::shared_ptr<VRONode> getParentNode() const {
        return _supernode.lock();
    }
    
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
     Remove all animations from this node.
     */
    void removeAllAnimations();

    /*
     Triggered when the animation running this animatable node completes.
     */
    void onAnimationFinished();
    
#pragma mark - Events 
    
    VROBoundingBox getBoundingBox() const;
    VROBoundingBox getUmbrellaBoundingBox() const;
    std::vector<VROHitTestResult> hitTest(const VROCamera &camera, VROVector3f origin, VROVector3f ray,
                                          bool boundsOnly = false);
    
    void setSelectable(bool selectable) {
        _selectable = selectable;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delegate) {
        passert_thread();
        
        _eventDelegateWeak = delegate;
    }

    std::shared_ptr<VROEventDelegate> getEventDelegate(){
        if (_eventDelegateWeak.expired()){
            return nullptr;
        }
        return _eventDelegateWeak.lock();
    }

    bool isSelectable() const {
        return _selectable;
    }

    void setTag(std::string tag) {
        _tag = tag;
    }

    std::string getTag() const {
        return _tag;
    }
    
    void setHighAccuracyGaze(bool enabled);
    
    bool getHighAccuracyGaze() const {
        return _highAccuracyGaze;
    }

    void setIsBeingDragged(bool isDragging) {
        _selectable = !isDragging;
        std::shared_ptr<VROPhysicsBody> physicsBody = getPhysicsBody();
        if (physicsBody != nullptr) {
            physicsBody->setKinematicDrag(isDragging);
        }
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

protected:
    
    VRONodeType _type;
    
    /*
     The node's parent and children.
     */
    std::vector<std::shared_ptr<VRONode>> _subnodes;
    std::weak_ptr<VRONode> _supernode;
    
    /*
     The geometry in the node. Null means the node has no geometry.
     */
    std::shared_ptr<VROGeometry> _geometry;
    
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
     Lights, sound, and camera.
     */
    std::vector<std::shared_ptr<VROLight>> _lights;
    std::vector<std::shared_ptr<VROSound>> _sounds;
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
     ancestors. Similarly, computedTransform is the full cascaded transformation 
     matrix for the node. 
     
     computedRotation only takes into account rotations (not scale or translation).
     computedLights are the lights that influence this node, based on distance from
     the light and light attenuation, unrelated to the scene graph (e.g. the lights
     in _computedLights may belong to any node in the scene).
     */
    VROMatrix4f _computedTransform;
    VROMatrix4f _computedInverseTransposeTransform;
    VROMatrix4f _computedRotation;
    float _computedOpacity;
    std::vector<std::shared_ptr<VROLight>> _computedLights;
    uint32_t _computedLightsHash;
    VROVector3f _computedPosition;
    std::weak_ptr<VROTransformDelegate> _transformDelegate;

    /*
     The transformed bounding box containing this node's geometry. The 
     _umbrellaBoundingBox encompasses not only this geometry, but the geometries
     of all this node's children.
     */
    VROBoundingBox _computedBoundingBox;
    VROBoundingBox _umbrellaBoundingBox;
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
     Delegate through which events are notified from the VROEventManager.
     */
    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    /*
     True if we want to perform more accurate hit testing against this node's geometry
     rather than its bounding box.
     */
    bool _highAccuracyGaze;

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
     should be rendered by order of their scene graph depth. Useful when rendering
     2D layouts like flexbox views. Defaults to false.
     */
    bool _hierarchicalRendering;
    
#pragma mark - Private
    
    /*
     Recursively set the visibility of this node and all of its children to the 
     given value.
     */
    void setVisibilityRecursive(bool visible);
    
    /*
     Recursively expand the given bounding box by this node's _computedBoundingBox.
     */
    void computeUmbrellaBounds(VROBoundingBox *bounds) const;
    
    /*
     Compute the transform for this node, taking into the account the parent's transform.
     Updates all related variables:
     
     _computedTransform
     _computedPosition
     _computedBoundingBox
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
    bool hitTestGeometry(VROVector3f origin, VROVector3f ray, VROMatrix4f transform);
    
    /*
     If the portal is inside-out, meaning during the last renderStencil() we wrote its parent's
     stencil bits, then this is true.
     */
    bool _portalInsideOut;

    /*
     Physics rigid body that if defined, drives and sets the transformations of this node.
     */
    std::shared_ptr<VROPhysicsBody> _physicsBody;

    /*
     Non-unique tag identifier representing this node. Defaults to kDefaultNodeTag.
     */
    std::string _tag = kDefaultNodeTag;
};

#endif /* VRONode_h */
