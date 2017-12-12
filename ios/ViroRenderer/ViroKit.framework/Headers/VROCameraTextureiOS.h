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
// #include "VROTrackingHelper.h"
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@class VROCameraCaptureDelegate;
@class VROCameraOrientationListener;
class VROVideoTextureCache;

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
    
    void displayPixelBuffer(std::unique_ptr<VROTextureSubstrate> substrate);
    void updateOrientation(VROCameraOrientation orientation);
    
private:
    
    /*
     Capture session and delegate used for live video playback.
     */
    AVCaptureSession *_captureSession;
    VROCameraCaptureDelegate *_delegate;
    VROCameraOrientationListener *_orientationListener;
    
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

// @property (nonatomic, strong) VROTrackingHelper* trackingHelper;

- (id)initWithCameraTexture:(std::shared_ptr<VROCameraTextureiOS>)texture
                      cache:(std::shared_ptr<VROVideoTextureCache>)cache
                     driver:(std::shared_ptr<VRODriver>)driver;

@end

/*
 Delegate for listening to orientation changes.
 */
@interface VROCameraOrientationListener : NSObject

- (id)initWithCameraTexture:(std::shared_ptr<VROCameraTextureiOS>)texture;
- (void)orientationDidChange:(NSNotification *)notification;

@end

#endif /* VROCameraTextureiOS_h */
