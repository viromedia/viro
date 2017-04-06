//
//  VROCameraTexture.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 3/22/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROCameraTexture_hpp
#define VROCameraTexture_hpp

#include "VROTexture.h"
#include <memory>

class VRORenderContext;
class VROFrameSynchronizer;
class VRODriver;

enum class VROCameraPosition {
    Front,
    Back
};

class VROCameraTexture : public VROTexture {
    
public:
    
    VROCameraTexture(VROTextureType type) :
        VROTexture(type) {}
    virtual ~VROCameraTexture() {}
    
    /*
     Initialize this texture to display the contents of the camera.
     The texture will not display until play() is invoked. Returns
     true if successful, false on failure.
     */
    virtual bool initCamera(VROCameraPosition position, std::shared_ptr<VRODriver> driver) = 0;
    
    /*
     Play and pause.
     */
    virtual void pause() = 0;
    virtual void play() = 0;
    virtual bool isPaused() = 0;
    
};

#endif /* VROCameraTexture_hpp */
