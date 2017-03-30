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
#import <AVFoundation/AVFoundation.h>

@class VROCameraCaptureDelegate;
class VROVideoTextureCache;

class VROCameraTextureiOS : public VROCameraTexture {
        
public:
        
    VROCameraTextureiOS(VROTextureType type);
    virtual ~VROCameraTextureiOS();
    
    bool initCamera(VROCameraPosition position, std::shared_ptr<VRODriver> driver);
    void pause();
    void play();
    bool isPaused();
    
    void displayPixelBuffer(std::unique_ptr<VROTextureSubstrate> substrate);
    
private:
    
    /*
     Capture session and delegate used for live video playback.
     */
    AVCaptureSession *_captureSession;
    VROCameraCaptureDelegate *_delegate;
    
    /*
     True if paused.
     */
    bool _paused;
    
    /*
     Video texture cache used for transferring camera content to OpenGL.
     */
    std::shared_ptr<VROVideoTextureCache> _videoTextureCache;

};

/*
 Delegate for capturing video from cameras.
 */
@interface VROCameraCaptureDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>

- (id)initWithCameraTexture:(std::shared_ptr<VROCameraTextureiOS>)texture
                      cache:(std::shared_ptr<VROVideoTextureCache>)cache;

@end

#endif /* VROCameraTextureiOS_h */
