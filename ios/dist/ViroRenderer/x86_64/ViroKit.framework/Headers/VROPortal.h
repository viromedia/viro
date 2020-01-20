//
//  VROPortal.h
//  ViroKit
//
//  Created by Raj Advani on 7/31/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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

#ifndef VROPortal_h
#define VROPortal_h

#include "VRONode.h"
#include "VROTree.h"
#include "VROLineSegment.h"
#include "VROPortalDelegate.h"

class VROPortalFrame;

/*
 Portals are nodes that partition subgraphs of the overall scene
 graph. They are used to simulate "teleportation" between two
 different scenes, and can also be used for more efficient visibility
 determination.
 */
class VROPortal : public VRONode {
public:
    
    VROPortal();
    virtual ~VROPortal();
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     
     Recurses down the tree.
     */
    void deleteGL();
    
    /*
     Render this portal's background.
     */
    void renderBackground(const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
    /*
     Render this portal's geometry.
     */
    void renderPortal(const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
    /*
     Render this portal's silhouette. The geometry is rendered using the provided material. This
     method is typically used to render into the stencil or depth buffers only.
     */
    void renderPortalSilhouette(std::shared_ptr<VROMaterial> &material,
                                VROSilhouetteMode mode, std::function<bool(const VRONode &)> filter,
                                const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
    /*
     Render the visible nodes in this portal's graph, in an order determined by the
     latest computed sort keys.
     */
    void renderContents(const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);
    
    /*
     Iterate up and down the scene graph, starting at the active portal.
     Fill in the recursion level of each portal (defined as steps to
     reach from the active portal), and return a graph of all portals reached.
     
     This is the recursive helper for VROScene::sortPortalsByDistance().
     
     The frame number if used as a 'visited' flag, as this function searches
     both up and down the graph and is expected to only be invoked once
     per frame as part of the render-cycle.
     
     The activeFrame node is used to assign an entrance frame geometry to this
     portal. See the discussion under _activePortalFrame for more detail.
     */
    void traversePortals(int frame, int recursionLevel,
                         std::shared_ptr<VROPortalFrame> activeFrame,
                         tree<std::shared_ptr<VROPortal>> *outPortals);
    
    /*
     Sort the visible nodes in this portal's sub-graph by their sort-keys, and fill
     the internal _keys vector with the results.
     */
    void sortNodesBySortKeys();
    
    /*
     Represents how how many levels deep this portal is: for example, the active portal
     is 0, a portal within the active portal is 1, a portal within a portal within a portal
     is 2, and so on.
     
     Note this is *always* relative to the active portal.
     */
    int getRecursionLevel() const {
        return _recursionLevel;
    }
    
    /*
     Set the node (with geometry) to render for the entrance to this portal. This
     geometry will double as the 'exit' back to this portal from children.
     */
    void setPortalEntrance(std::shared_ptr<VROPortalFrame> entrance);
    const std::shared_ptr<VROPortalFrame> getPortalEntrance() {
        return _portalEntrance;
    }
    
    /*
     Return the portal frame being rendered for this portal; either an exit or an
     entrance.
     */
    const std::shared_ptr<VROPortalFrame> getActivePortalFrame() const {
        return _activePortalFrame;
    }
    
    /*
      Return portalDelegate if it exists.
     */
    std::shared_ptr<VROPortalDelegate> getPortalDelegate() {
        if (_portalDelegate.expired()){
            return nullptr;
        }
        return _portalDelegate.lock();
    }
    
    void setPortalDelegate(std::shared_ptr<VROPortalDelegate> delegate) {
        _portalDelegate = delegate;
    }
    
    /*
     Return true if the portal frame rendered by this portal is an exit into
     a parent portal, as opposed to this portal's own 'entrance' portal.
     */
    bool isRenderingExitFrame() const {
        return _activePortalFrame != _portalEntrance;
    }
    
    /*
     If a portal is passable then it reprsents an entry-point into another navigable world,
     meaning it can be set as the active portal. Non-passable portals can be AR elements overlaid
     on the real world like picture frames, that are meant to be viewed from outside only.
     */
    void setPassable(bool passable) {
        _passable = passable;
    }
    bool isPassable() const {
        return _passable;
    }
    
    /*
     Return true if the given line segment intersects the portal geometry.
     */
    bool intersectsLineSegment(VROLineSegment segment) const;
    
#pragma mark - Environment Lighting
    
    /*
     Set the lighting environment map for this portal.
     */
    void setLightingEnvironment(std::shared_ptr<VROTexture> texture);
    
    /*
     Get the lighting environment map for this portal.
     */
    std::shared_ptr<VROTexture> getLightingEnvironment() const;
    
#pragma mark - Backgrounds
    
    /*
     Note: the scene renders all backgrounds in its tree before rendering
     any content.
     */
    
    /*
     Set the background to a cube-map defined by the given cube texture or
     color.
     */
    void setBackgroundCube(std::shared_ptr<VROTexture> textureCube);
    void setBackgroundCube(VROVector4f color);
    
    /*
     Set the background to a textured sphere.
     */
    void setBackgroundSphere(std::shared_ptr<VROTexture> textureSphere);
    
    /*
     Set the background to an arbitrary geometry. All this guarantees is that
     the given object will be rendered first. No properties will be set on
     this geometry, but typically background geometries are screen-space, and
     do not read or write to the depth buffer.
     */
    void setBackground(std::shared_ptr<VROGeometry> geometry);
    
    /*
     Set an arbitrary transform to apply to the background. The transform
     may also be set as a quaternion (rotation).
     */
    void setBackgroundTransform(VROMatrix4f transform);
    void setBackgroundRotation(VROQuaternion rotation);
    
    std::shared_ptr<VROGeometry> getBackground() const {
        return _background;
    }
    void removeBackground();
    
private:
    
    /*
     The number of steps to reach this portal from the active portal.
     */
    int _recursionLevel;
    
    /*
     The nodes in this portal's scene-graph, ordered for rendering by their
     sort keys.
     */
    std::vector<VROSortKey> _keys;
    
    /*
     True if this portal can be entered; e.g, if it can be made into an
     active portal.
     */
    bool _passable;
    
    /*
     Portals have an entrance node that strictly defines the *geometry*
     (the frame, or window) that delineates the entrance into, or exit from,
     the portal.
     */
    std::shared_ptr<VROPortalFrame> _portalEntrance;
    
    /*
     The _activePortalFrame is the frame this portal should render as its
     entrance.
     
     When moving down the portal tree, this is set to the portal's own
     entrance: _portalEntrance. However, when moving up the tree, this is
     set to the active child portal's entrance.
     
     For example, if we have the following portal tree:
     
         A
       B   E
     C   D
     
     If A is the active portal, then B would render its own entrance (as an
     entrance from A into B), C would render its own entrance (as an entrance
     from B into C) and so on for every portal in the tree.
     
     However, if D is the active portal, then B will render D's entrance (as
     an exit from D into B), B will render A's entrance (as an exit from B into
     A), and the remaining portals (C and E) would simply render their own
     entrances.
     
     These portals frames are assigned during traversePortals.
     */
    std::shared_ptr<VROPortalFrame> _activePortalFrame;
    
    /*
     The background visual to display. All backgrounds in the scene are rendered before
     node content.
     */
    std::shared_ptr<VROGeometry> _background;
    
    /*
     The lighting environment for this portal. Determines the effect of image-based
     lighting (IBL) for objects using the physically based lighting model.
     */
    std::shared_ptr<VROTexture> _lightingEnvironment;
    
    /*
      Portal delegate that is invoked when a portal is entered and exited.
     */
    std::weak_ptr<VROPortalDelegate> _portalDelegate;
    
    /*
     Transform to apply to the background geometry.
     */
    VROMatrix4f _backgroundTransform;
    
    /*
     Installs required shader modifiers on the background.
     */
    void installBackgroundShaderModifier();
    
    /*
     Deactivates culling on every geometry in the given node, recursively down the
     tree. Needed to ensure culling is off on portal frames.
     */
    void deactivateCulling(std::shared_ptr<VRONode> node);

    /*
     Write the hierarchy parent represented by the given sort key to the depth buffer
     (only).
     */
    void writeHierarchyParentToDepthBuffer(VROSortKey &hierarchyParent,
                                           const VRORenderContext &context,
                                           std::shared_ptr<VRODriver> &driver);
    
};

#endif /* VROPortal_h */
