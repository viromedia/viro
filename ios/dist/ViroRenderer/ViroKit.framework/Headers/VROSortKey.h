//
//  VROSortKey.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 6/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSortKey_hpp
#define VROSortKey_hpp

#include <stdio.h>
#include <tuple>

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
         The std::tie operation perform the sort for us, in order of increasing important.
         We generally sort by rendering order and distance to camera, then by batch switching
         concerns (shader, textures, light, material), and finally by tie-breakers (node, 
         element index).
         
         For hierarchies, note that the distance from camera for all objects in a hierarchy
         is set to the distance from camera of the parent. This way distance from camera becomes
         irrelevant within a hierarchy, so that within each hierarchy we can sort by hierarchy
         depth only. Note we do not sort by hierarchy ID because it is ok to interleave objects
         of different hierarchies within the render order.
         */
        return std::tie(renderingOrder, distanceFromCamera, hierarchyDepth, incoming, shader, textures, lights, material, node, elementIndex) <
               std::tie(r.renderingOrder, r.distanceFromCamera, r.hierarchyDepth, r.incoming, r.shader, r.textures, r.lights, r.material, r.node, r.elementIndex);
    }
            
    /*
     Manual rendering order setting is the highest sorting concern.
     */
    uint32_t renderingOrder;
    
    /*
     The depth of the node in its hierarchy. This is normally set
     to zero. If any of the node's parents is set to hierarchical 
     rendering, then this is set to the node's depth in the hierarchy,
     with 0 being the depth of the first hierarhical parent, 1 being
     the depth after, etc.
     
     This ensures that when a hierarchial node is rendered, all 
     of its children will be rendered immediately after by order of 
     their depth.
     */
    uint32_t hierarchyDepth;
    
    /*
     The ID of the hierarchy this node resides in. When comparing two
     nodes, we only include graph depth in the sort if the two nodes
     are in the same hierarchy. 0 indicates we are not in a hierarchy.
     */
    uint32_t hierarchyId;
    
    /*
     Distance fom camera for objects is next (back to front).
     This value is set to (zFar - distance from the camera).
     
     When sorted, this results in back to front rendering of
     objects, ensuring correct rendering of translucent objects.
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
    uint8_t elementIndex;
    
};

// Uncomment to see a compiler error indicating the size of each VROSortKey
// template<int s> struct SortKeySize;
// SortKeySize<sizeof(VROSortKey)> sortKeySize;

#endif /* VROSortKey_hpp */
