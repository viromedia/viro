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
        return std::tie(renderingOrder, distanceFromCamera, shader, textures, lights, material, node, elementIndex, outgoing) <
               std::tie(r.renderingOrder, r.distanceFromCamera, r.shader, r.textures, r.lights, r.material, r.node, r.elementIndex, r.outgoing);
    }
            
    /*
     Manual rendering order setting is the highest sorting concern.
     */
    uint32_t renderingOrder;
    
    /*
     Distance fom camera for objects is next (back to front).
     This value is set to (zFar - distance from the camera).
     
     When sorted, this results in back to front rendering of
     objects, ensuring correct rendering of translucent objects.
     */
    float distanceFromCamera;
    
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
    bool outgoing;
    
};

// Uncomment to see a compiler error indicating the size of each VROSortKey
// template<int s> struct SortKeySize;
// SortKeySize<sizeof(VROSortKey)> sortKeySize;

#endif /* VROSortKey_hpp */
