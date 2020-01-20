//
//  VROCameraTextureiOS.h
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

#ifndef VROCameraTextureiOS_h
#define VROCameraTextureiOS_h

#include "VROCameraTexture.h"
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@class VROCameraCaptureDelegate;
@class VROCameraOrientationListener;
class VROVideoTextureCache;
class VROAVCaptureController;

class VROCameraTextureiOS : public VROCameraTexture {
        
public:
        
    VROCameraTextureiOS(VROTextureType type);
    virtual ~VROCameraTextureiOS();
    
    bool initCamera(VROCameraPosition position, VROCameraOrientation orientation, std::shared_ptr<VRODriver> driver);
    void pause();
    void play();
    bool isPaused();
    
    float getHorizontalFOV() const;
    VROVector3f getImageSize() const;
    
    /*
     Get the CMSampleBufferRef that corresponds to the current image displayed on
     this texture.
     */
    CMSampleBufferRef getSampleBuffer() const;
    
    /*
     Get the camera intrinsics that correspond to the last image displayed on this
     texture.
     */
    std::vector<float> getCameraIntrinsics() const;
    
private:
    
    /*
     Capture controller used for live video playback.
     */
    std::shared_ptr<VROAVCaptureController> _controller;
    
    /*
     Video texture cache used for transferring camera content to OpenGL.
     */
    std::shared_ptr<VROVideoTextureCache> _videoTextureCache;

};

#endif /* VROCameraTextureiOS_h */
