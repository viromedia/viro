//
//  VROSortKey.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 6/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROSortKey_hpp
#define VROSortKey_hpp

#include <stdio.h>
#include <tuple>

static const int kMaxHierarchyId = 100;

/*
 Sort keys are used to quickly sort geometry elements into optimal batch rendering order,
 to limit state changes on the GPU. They consist of a large byte[] that can be sorted
 quickly via memcmp.
 */
class VROSortKey {
    
public:
    VROSortKey() {
        
    }

    bool operator< (const VROSortKey& r) const {
        /*
         The std::tie operation perform the sort for us, in order of increasing importance.
         We generally sort by rendering order, opacity (opaque objects first), and
         distance to camera, then by batch switching concerns (shader, textures, light,
         material), and finally by tie-breakers (node,  element index).
         
         For hierarchies, note that the distance from camera for all objects in a hierarchy
         is set to the distance from camera of the parent. This way distance from camera becomes
         irrelevant within a hierarchy, so that within each hierarchy we can sort by hierarchy
         depth only. We also sort by hierarchy ID because we want the entirely of a hierarchy to
         appear continuously in the sort order; this is essential for the deferred depth-write
         rendering to work (see VROPortal::renderContents).

         Note that because hierarchies appear before transparency in the sort order (they must,
         otherwise the hierarchies would not be continuous in the sort order), this means
         hierarchies will not *always* work with transparency. We make hierarchies render first,
         so that they will appear behind other transparent objects. But *transparent* hierarchies
         will not display opaque objects behind them. This is a known limitation with this system.
         Before attempting to fix, look through the git history of this file (it has been tried
         before).
         */
        return std::tie(renderingOrder, hierarchyId, hierarchyDepth, transparent, distanceFromCamera, incoming, materialRenderingOrder, shader, textures, lights, material, node, elementIndex) <
               std::tie(r.renderingOrder, r.hierarchyId, r.hierarchyDepth, r.transparent, r.distanceFromCamera, r.incoming, r.materialRenderingOrder, r.shader, r.textures, r.lights, r.material, r.node, r.elementIndex);
    }
            
    /*
     Manual rendering order setting (set from VRONode) is the highest sorting concern.
     */
    int32_t renderingOrder;

    /*
     The depth of the node in its hierarchy. This is normally set
     to zero. If any of the node's parents is set to hierarchical 
     rendering, then this is set to the node's depth in the hierarchy,
     with 0 being the depth of the first hierarchical parent, 1 being
     the depth after, etc.
     
     This ensures that when a hierarchial node is rendered, all 
     of its children will be rendered immediately after by order of 
     their depth.
     */
    uint32_t hierarchyDepth;
    
    /*
     The ID of the hierarchy this node resides in. When comparing two
     nodes, we only include graph depth in the sort if the two nodes
     are in the same hierarchy. In order to make hierarchies render prior
     to other objects, this value is inverted: it is set to kMaxHierarchyId - hierarchyId.
     Therefore, a hierarchyId of kMaxHierarchyId indicates no hierarchy.
     */
    uint32_t hierarchyId;

    /*
     True if the object is (full or semi) transparent. Opaque objects are
     rendered first, so that they appear correctly when transparent objects are rendered
     in front of them.
     */
    bool transparent;
    
    /*
     Distance from camera for objects is next (back to front).
     This value is set to (zFar - distance from the camera).
     
     When sorted, this results in back to front rendering of
     objects, ensuring correct rendering of transparent objects.
     */
    float distanceFromCamera;

    /*
     If a material is incoming (i.e. geometry transitioning in to
     a new material as a result of an animation), we render it after
     its outgoing counterpart. The incoming material renders
     second (on top), as its alpha moves from 0 to 1.
    */
    bool incoming;

    /*
     Material rendering order is the last render-order concern; it only involves materials that use
     the same geometry (meaning all previous values above are the same).
     */
    uint32_t materialRenderingOrder;
    
    /*
     State-change minimization concerns.
     */
    uint32_t shader;
    uint32_t textures;
    uint32_t lights;
    uint32_t material;
    
    //// 24 bytes to this point. Keep uintptr_t at an 8 byte boundary to avoid waste ////
    
    /*
     Tie-breakers, double as pointers to the node and
     index of the geometry element.
     */
    uintptr_t node;
    uint32_t elementIndex;
    
};

// Uncomment to see a compiler error indicating the size of each VROSortKey
// template<int s> struct SortKeySize;
// SortKeySize<sizeof(VROSortKey)> sortKeySize;

#endif /* VROSortKey_hpp */
