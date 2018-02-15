//
//  VROConcurrentBuffer.h
//  ViroRenderer
//
//  Created by Raj Advani on 2/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROConcurrentBuffer_h
#define VROConcurrentBuffer_h

#include "VRODefines.h"
#if VRO_METAL

#include <stdio.h>
#include "VROEye.h"
#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

static const int kMaxBuffersInFlight = 3;

/*
 Wraps a Metal buffer that's used by both the GPU and the CPU.
 The section of the buffer being used (written to by the CPU) changes
 with each frame, so that the CPU and GPU never collide.
 */
class VROConcurrentBuffer {
    
public:
    
    VROConcurrentBuffer(int size, NSString *label, id <MTLDevice> device);
    virtual ~VROConcurrentBuffer();
    
    /*
     Get the underlying MTLBuffer.
     */
    id <MTLBuffer> getMTLBuffer(VROEyeType eye) {
        return _buffer[(int)eye];
    }
    
    void *getWritableContents(VROEyeType eye, int frame) {
        return (void *) ((char *)[_buffer[(int)eye] contents] + getWriteOffset(frame));
    }
    
    /*
     Get the CPU write offset into the MTLBuffer, based on the current
     frame.
     */
    int getWriteOffset(int frame) {
        return _size * (frame % kMaxBuffersInFlight);
    }
    
private:
    
    /*
     The underlying Metal buffer for each eye.
     */
    id <MTLBuffer> _buffer[2];
    
    /*
     The size of each section in the MTLBuffer.
     */
    int _size;
    
};

#endif
#endif /* VROConcurrentBuffer_h */
