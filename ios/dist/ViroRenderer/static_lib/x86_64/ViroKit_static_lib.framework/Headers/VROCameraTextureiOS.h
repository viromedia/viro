//
//  VROCameraTextureiOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/22/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
