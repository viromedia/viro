//
//  VRORenderToTextureDelegate.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VRO_RENDER_TO_TEXTURE_DELEGATE_H
#define VRO_RENDER_TO_TEXTURE_DELEGATE_H

#include <memory>

class VRORenderTarget;
class VRODriver;
class VROViewport;

/*
 The VRORenderToTextureDelegate is notified at the end of each frame with a VRORenderTarget
 containing the fully rendered scene. This can be used to pass the rendered scene on to
 other processes.
 */
class VRORenderToTextureDelegate : public std::enable_shared_from_this<VRORenderToTextureDelegate> {
public:
    VRORenderToTextureDelegate() {};
    virtual ~VRORenderToTextureDelegate() {};

    /*
     Called to notify delegates that a frame has been rendered to the given renderedTarget,
     which contains the final output of the scene.
     */
    virtual void didRenderFrame(std::shared_ptr<VRORenderTarget> renderedTarget,
                                std::shared_ptr<VRODriver> driver) = 0;
};

#endif //VRO_RENDER_TO_TEXTURE_DELEGATE_H
