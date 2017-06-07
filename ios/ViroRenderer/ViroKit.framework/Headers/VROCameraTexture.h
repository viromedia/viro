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
class VROVector3f;

enum class VROCameraPosition {
    Front,
    Back
};

enum class VROCameraOrientation {
    Portrait,
    PortraitUpsideDown,
    LandscapeLeft,
    LandscapeRight,
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
    virtual bool initCamera(VROCameraPosition position, VROCameraOrientation orientation,
                            std::shared_ptr<VRODriver> driver) = 0;
    
    /*
     Play and pause.
     */
    virtual void pause() = 0;
    virtual void play() = 0;
    virtual bool isPaused() = 0;
    
    /*
     Get the horizontal FOV used by the camera. This should return the
     FOV from edge to edge, in degrees.
     */
    virtual float getHorizontalFOV() const = 0;
    
    /*
     Get the image size (width, height) for the camera. Stored in the
     vector's x and y components.
     */
    virtual VROVector3f getImageSize() const = 0;
    
};

#endif /* VROCameraTexture_hpp */
