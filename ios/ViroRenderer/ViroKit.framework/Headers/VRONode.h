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
#include <algorithm>
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

class VROGeometry;
class VROLight;
class VROAction;
class VROHitTestResult;
class VROConstraint;

class VRONode : public VROAnimatable {
    
public:
    
    /*
     Designated initializer for nodes in the model tree.
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

    void render(int elementIndex,
                std::shared_ptr<VROMaterial> &material,
                const VRORenderContext &context,
                VRODriver &driver);
    
    void updateSortKeys(VRORenderParameters &params,
                        const VRORenderContext &context,
                        VRODriver &driver);
    void getSortKeys(std::vector<VROSortKey> *outKeys);
    
    std::vector<std::shared_ptr<VROLight>> &getComputedLights() {
        return _computedLights;
    }
    
    void setGeometry(std::shared_ptr<VROGeometry> geometry) {
        _geometry = geometry;
    }
    std::shared_ptr<VROGeometry> getGeometry() const {
        return _geometry;
    }
    
    /*
     Transforms.
     */
    void computeTransform(const VRORenderContext &context, VROMatrix4f parentTransforms);
    VROVector3f getTransformedPosition() const;
    
    VROVector3f getPosition() const {
        return _position;
    }
    VROVector3f getScale() const {
        return _scale;
    }
    VROVector3f getPivot() const {
        return _pivot;
    }
    VROQuaternion getRotation() const {
        return _rotation;
    }
    
    void setRotation(VROQuaternion rotation);
    void setPosition(VROVector3f position);
    void setScale(VROVector3f scale);
    
    float getOpacity() const {
        return _opacity;
    }
    void setOpacity(float opacity);

    void setHighAccuracyGaze(bool enabled);

    bool getHighAccuracyGaze() const {
        return _highAccuracyGaze;
    }

    bool isHidden() const {
        return _hidden;
    }
    void setHidden(bool hidden);
    
    int getRenderingOrder() const {
        return _renderingOrder;
    }
    void setRenderingOrder(int renderingOrder) {
        _renderingOrder = renderingOrder;
    }
    
    /*
     The pivot point is the point about which we apply rotation, translation,
     and scale. This is specified as a vector ranging from [0, 1], where 0 
     corresponds to the leftmost point (bottom-most, nearest) point in the 
     node's local coordinate system, and 1.0 corresponds to the rightmost 
     (top-most, furthest) point in the local coordinate system.
     
     Default is {0.5, 0,5. 0,5} which corresponds the center of the node.
     */
    void setPivot(VROVector3f pivot);
    
    /*
     Lights.
     */
    void addLight(std::shared_ptr<VROLight> light) {
        _lights.push_back(light);
    }
    void removeLight(std::shared_ptr<VROLight> light) {
        _lights.erase(
                      std::remove_if(_lights.begin(), _lights.end(),
                                     [light](std::shared_ptr<VROLight> candidate) {
                                         return candidate == light;
                                     }), _lights.end());
    }
    void removeAllLights() {
        _lights.clear();
    }
    std::vector<std::shared_ptr<VROLight>> &getLights() {
        return _lights;
    }
    
    /*
     Child management.
     */
    void addChildNode(std::shared_ptr<VRONode> node) {
        passert (node);
        
        _subnodes.push_back(node);
        node->_supernode = std::static_pointer_cast<VRONode>(shared_from_this());
    }
    void removeFromParentNode() {
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
    std::vector<std::shared_ptr<VRONode>> getSubnodes() const {
        return _subnodes;
    }
    
    /*
     Return the parent node. Null if this node is root or does not have a parent.
     */
    std::shared_ptr<VRONode> getParentNode() const {
        return _supernode.lock();
    }
    
    /*
     Action management.
     */
    void runAction(std::shared_ptr<VROAction> action);
    void removeAction(std::shared_ptr<VROAction> action);
    void removeAllActions();
    
    /*
     Hit testing.
     */
    VROBoundingBox getBoundingBox(const VRORenderContext &context);
    std::vector<VROHitTestResult> hitTest(VROVector3f ray, VROVector3f origin,
                                          bool boundsOnly = false);
    
    void setSelectable(bool selectable) {
        _selectable = selectable;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delgate){
        auto autoWeakDelegate = delgate;
        _eventDelegateWeak = autoWeakDelegate;
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
    
    /*
     Constraints.
     */
    void addConstraint(std::shared_ptr<VROConstraint> constraint);
    void removeConstraint(std::shared_ptr<VROConstraint> constraint);
    void removeAllConstraints();
    
protected:
    
    /*
     The node's parent and children.
     */
    std::vector<std::shared_ptr<VRONode>> _subnodes;
    std::weak_ptr<VRONode> _supernode;
    
private:
    
    std::shared_ptr<VROGeometry> _geometry;
    std::vector<std::shared_ptr<VROLight>> _lights;
    
    VROVector3f _scale;
    VROVector3f _position;
    VROQuaternion _rotation;
    VROVector3f _pivot;
    
    /*
     User-defined rendering order for this node.
     */
    int _renderingOrder;
    
    /*
     Parameters computed by descending down the tree. These are updated whenever
     any parent or this node itself is updated.
     */
    VROMatrix4f _computedTransform;
    VROMatrix4f _computedInverseTransposeTransform;
    float _computedOpacity;
    std::vector<std::shared_ptr<VROLight>> _computedLights;
    VROVector3f _computedPosition;
    
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

    /**
     * Delegate through which events are notified from the VROEventManager.
     */
    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    /*
     True if we want to perform more accurate hit testing against this node's geometry
     rather than it's bounding box.
     */
    bool _highAccuracyGaze;

    /*
     Active actions on this node.
     */
    std::vector<std::shared_ptr<VROAction>> _actions;
    
    /*
     Constraints on the node, which can modify the node's transformation matrix.
     */
    std::vector<std::shared_ptr<VROConstraint>> _constraints;
    
    /*
     Action processing: execute all current actions and remove those that are
     expired.
     */
    void processActions();
    
    /*
     Hit test helper functions.
     */
    void hitTest(VROVector3f ray,  VROMatrix4f parentTransform,  bool boundsOnly,
                 VROVector3f origin, std::vector<VROHitTestResult> &results);
    bool hitTestGeometry(VROVector3f ray, VROVector3f origin, VROMatrix4f transform);

};

#endif /* VRONode_h */
