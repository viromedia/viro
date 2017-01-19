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
        return std::tie(renderingOrder, graphDepth, distanceFromCamera, incoming, shader, textures, lights, material, node, elementIndex) <
               std::tie(r.renderingOrder, r.graphDepth, r.distanceFromCamera, r.incoming, r.shader, r.textures, r.lights, r.material, r.node, r.elementIndex);
    }
            
    /*
     Manual rendering order setting is the highest sorting concern.
     */
    uint32_t renderingOrder;
    
    /*
     The depth of the node in the scene graph. This is normally set
     to zero. If any of the node's parents is set to hierarchical 
     rendering, then this is set to the node's depth in the graph.
     
     This ensures that when a hierarchial node is rendered, all 
     of its children will be rendered immediately after by order of 
     their depth.
     */
    uint32_t graphDepth;
    
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
