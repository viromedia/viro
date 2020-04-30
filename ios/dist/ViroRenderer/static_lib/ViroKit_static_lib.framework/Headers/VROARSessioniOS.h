//
//  VROARSessioniOS.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
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

#ifndef VROARSessioniOS_h
#define VROARSessioniOS_h

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110000
#include "VROARSession.h"
#include "VROViewport.h"
#include <ARKit/ARKit.h>
#include <map>
#include <vector>

#include "VRORenderer.h"

class VRODriver;
class VROVideoTextureCacheOpenGL;
class VROTrackingHelper;
@class VROARKitSessionDelegate;

class API_AVAILABLE(ios(12.0)) VROARSessioniOS : public VROARSession, public std::enable_shared_from_this<VROARSessioniOS> {
public:
    
    VROARSessioniOS(VROTrackingType trackingType,
                    VROWorldAlignment worldAlignment,
                    std::shared_ptr<VRODriver> driver);
    virtual ~VROARSessioniOS();
    
    void setTrackingType(VROTrackingType trackingType);

    void run();
    void pause();
    bool isReady() const;
    void resetSession(bool resetTracking, bool removeAnchors);
    
    void setScene(std::shared_ptr<VROScene> scene);
    void setDelegate(std::shared_ptr<VROARSessionDelegate> delegate);
    bool setAnchorDetection(std::set<VROAnchorDetection> types);
    void setCloudAnchorProvider(VROCloudAnchorProvider provider);

    void loadARImageDatabase(std::shared_ptr<VROARImageDatabase> arImageDatabase) {
        // unsupported
    }
    
    void unloadARImageDatabase() {
        // unsupported
    }

    void addARImageTarget(std::shared_ptr<VROARImageTarget> target);
    void removeARImageTarget(std::shared_ptr<VROARImageTarget> target);
    void addARObjectTarget(std::shared_ptr<VROARObjectTarget> target);
    void removeARObjectTarget(std::shared_ptr<VROARObjectTarget> target);
    void addAnchor(std::shared_ptr<VROARAnchor> anchor);
    void removeAnchor(std::shared_ptr<VROARAnchor> anchor);
    void hostCloudAnchor(std::shared_ptr<VROARAnchor> anchor,
                         std::function<void(std::shared_ptr<VROARAnchor>)> onSuccess,
                         std::function<void(std::string error)> onFailure);
    void resolveCloudAnchor(std::string anchorId,
                            std::function<void(std::shared_ptr<VROARAnchor> anchor)> onSuccess,
                            std::function<void(std::string error)> onFailure);
    
    std::unique_ptr<VROARFrame> &updateFrame();
    std::unique_ptr<VROARFrame> &getLastFrame();
    std::shared_ptr<VROTexture> getCameraBackgroundTexture();
    
    void setViewport(VROViewport viewport);
    void setOrientation(VROCameraOrientation orientation);
    
    void setWorldOrigin(VROMatrix4f relativeTransform);
    void setNumberOfTrackedImages(int numImages);
    void setAutofocus(bool enabled);
    bool isCameraAutoFocusEnabled() {
      // unsupported
      return false;
    }
    void setVideoQuality(VROVideoQuality quality);
    void setVisionModel(std::shared_ptr<VROVisionModel> visionModel);
    
    /*
     Internal methods.
     */
    void setFrame(ARFrame *frame);
    std::shared_ptr<VROARAnchor> getAnchorForNative(ARAnchor *anchor);
    void updateAnchor(std::shared_ptr<VROARAnchor> anchor);

    void addAnchor(ARAnchor *anchor);
    void updateAnchor(ARAnchor *anchor);
    void removeAnchor(ARAnchor *anchor);

private:
    
    /*
     The ARKit session, configuration, and delegate.
     */
    ARSession *_session;
    ARConfiguration *_sessionConfiguration;
    VROARKitSessionDelegate *_delegateAR;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110300
    NSMutableSet<ARReferenceImage *> *_arKitImageDetectionSet;
#endif

    /*
     Whether or not the session has been paused.
     */
    bool _sessionPaused;

    /*
     The last computed ARFrame.
     */
    std::unique_ptr<VROARFrame> _currentFrame;
    
    /*
     The current viewport and camera orientation.
     */
    VROViewport _viewport;
    VROCameraOrientation _orientation;
    
    /*
     Vector of all anchors that have been added to this session.
     */
    std::vector<std::shared_ptr<VROARAnchor>> _anchors;
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110300
    /*
     Map of ARReferenceImage ObjC objects to astd::shared_ptr<VROARImageTarget>
     that was used to create the ARReferenceImage.
     */
    std::map<ARReferenceImage *, std::shared_ptr<VROARImageTarget>> _arKitReferenceImageMap;
#endif
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 120000
    /*
     Set of ARReferenceObjects to detect
     */
    NSMutableSet<ARReferenceObject *> *_arKitObjectDetectionSet;

    /*
     Map of ARReferenceObject ObjC objects to a std::shared_ptr<VROARObjectTarget>
     that was used to create the ARReferenceObject.
     */
    std::map<ARReferenceObject *, std::shared_ptr<VROARObjectTarget>> _arKitReferenceObjectMap;
#endif
    
    /*
     Map of ARKit anchors ("native" anchors) to their Viro representation. 
     Required so we can update VROARAnchors when their ARKit counterparts are
     updated. Note that not all VROARAnchors have an ARKit counterpart (e.g. 
     they may be added and maintained by other tracking software).
     */
    std::map<std::string, std::shared_ptr<VROARAnchor>> _nativeAnchorMap;
    
    /*
     Background to be assigned to the VROScene.
     */
    std::shared_ptr<VROTexture> _background;
    
    /*
     Video texture cache used for transferring camera content to OpenGL.
     */
    std::shared_ptr<VROVideoTextureCacheOpenGL> _videoTextureCache;
    
    /*
     Update the VROARAnchor with the transforms in the given ARAnchor.
     */
    void updateAnchorFromNative(std::shared_ptr<VROARAnchor> vAnchor, ARAnchor *anchor);
    
    std::shared_ptr<VROVisionModel> _visionModel;
    
    void updateTrackingType(VROTrackingType trackingType);
    
};

/*
 Delegate for ARKit's ARSession.
 */
API_AVAILABLE(ios(12.0)) @interface VROARKitSessionDelegate : NSObject<ARSessionDelegate> 

- (id)initWithSession:(std::shared_ptr<VROARSessioniOS>)session;

@end

#endif
#endif /* VROARSessioniOS_h */
