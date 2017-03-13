//
//  VROVideoTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoSurface_h
#define VROVideoSurface_h

#include "VROSurface.h"
#include "VROVideoDelegateInternal.h"
#include <memory>

class VRORenderContext;
class VRODriver;
class VROMaterial;
class VROSurface;
class VROVideoTexture;
class VROFrameSynchronizer;

class VROVideoSurface : public VROSurface {
    
public:
    
    static std::shared_ptr<VROVideoSurface> createVideoSurface(float width, float height,
                                                               std::string url,
                                                               std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                                                               std::shared_ptr<VROVideoTexture> texture,
                                                               VRODriver &driver);
    
    void pause();
    void play();
    bool isPaused();
    void setMuted(bool muted);
    void setVolume(float volume);
    void setLoop(bool loop);
    void seekToTime(int seconds);
    void setDelegate(std::shared_ptr<VROVideoDelegateInternal> delegate);
  
    ~VROVideoSurface();
    
private:
    
    VROVideoSurface(std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                    std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                    std::shared_ptr<VROVideoTexture> texture);
    
    std::shared_ptr<VROVideoTexture> _texture;
    
};

#endif /* VROVideoSurface_h */
