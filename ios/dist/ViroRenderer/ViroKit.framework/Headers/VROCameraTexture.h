//
//  VROCameraTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/22/17.
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

#ifndef VROCameraTexture_h
#define VROCameraTexture_h

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
        VROTexture(type, VROTextureInternalFormat::RGBA8) {}
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
    
    /*
     Get the camera intrinsics that correspond to the last image displayed on this
     texture.
     */
    virtual std::vector<float> getCameraIntrinsics() const = 0;
    
};

#endif /* VROCameraTexture_h */
