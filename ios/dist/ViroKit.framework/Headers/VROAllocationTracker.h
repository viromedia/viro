//
//  VROAllocationTracker.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef __VROAllocationTracker__
#define __VROAllocationTracker__

#include <iostream>

#define TRACK_MEMORY_ALLOCATIONS 1

#if TRACK_MEMORY_ALLOCATIONS
    #define ALLOCATION_TRACKER_SET(x, bytes) VROAllocationTracker::set(VROAllocationBucket::x, bytes)
    #define ALLOCATION_TRACKER_ADD(x, bytes) VROAllocationTracker::add(VROAllocationBucket::x, bytes)
    #define ALLOCATION_TRACKER_SUB(x, bytes) VROAllocationTracker::subtract(VROAllocationBucket::x, bytes)
    #define ALLOCATION_TRACKER_RESIZE(x, bytesOld, bytesNew) VROAllocationTracker::resize(VROAllocationBucket::x, bytesOld, bytesNew)
    #define ALLOCATION_TRACKER_PRINT() VROAllocationTracker::print()
    #define ALLOCATION_TRACKER_PRINT_NOW() VROAllocationTracker::printNow()

    #define VRO_MALLOC(x, bytes) malloc(bytes), ALLOCATION_TRACKER_ADD(x, bytes)
    #define VRO_FREE(x, ptr, bytes) free(ptr), ALLOCATION_TRACKER_SUB(x, bytes)
    #define VRO_REALLOC(x, ptr, bytesOld, bytesNew) realloc(ptr, bytesNew), ALLOCATION_TRACKER_RESIZE(x, bytesOld, bytesNew)
#else
    #define ALLOCATION_TRACKER_SET(x, bytes) ((void)0)
    #define ALLOCATION_TRACKER_ADD(x, bytes) ((void)0)
    #define ALLOCATION_TRACKER_SUB(x, bytes) ((void)0)
    #define ALLOCATION_TRACKER_RESIZE(x, bytesOld, bytesNew) ((void)0)
    #define ALLOCATION_TRACKER_PRINT() ((void)0)
    #define ALLOCATION_TRACKER_PRINT_NOW() ((void)0)
    #define VRO_MALLOC(x, bytes) malloc(bytes)
    #define VRO_FREE(x, ptr, bytes) free(ptr)
    #define VRO_REALLOC(x, ptr, bytesOld, bytesNew) realloc(ptr, bytesNew)
#endif

enum class VROAllocationBucket {
    Scenes,
    Nodes,
    Geometry,
    Materials,
    MaterialSubstrates,
    Textures,
    TextureSubstrates,
    Shaders,
    ShaderModifiers,
    VideoTextures,
    NUM_BUCKETS
};

enum class VROLayerType : int8_t;

class VROAllocationTracker {

public:
    static void set(VROAllocationBucket bucket, uint32_t bytes);
    static void add(VROAllocationBucket bucket, uint32_t bytes);
    static void subtract(VROAllocationBucket bucket, uint32_t bytes);
    static void resize(VROAllocationBucket bucket, uint32_t bytesOld, uint32_t bytesNew);

    static void print();
    static void printNow();

};

#endif /* defined(__VROAllocationTracker__) */
