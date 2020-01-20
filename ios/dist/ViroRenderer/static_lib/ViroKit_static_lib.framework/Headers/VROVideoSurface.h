//
//  VROVideoTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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
                                                               std::shared_ptr<VRODriver> driver);
    
    void pause();
    void play();
    bool isPaused();
    void setMuted(bool muted);
    void setVolume(float volume);
    void setLoop(bool loop);
    void seekToTime(float seconds);
    void setDelegate(std::shared_ptr<VROVideoDelegateInternal> delegate);
  
    ~VROVideoSurface();
    
private:
    
    VROVideoSurface(float width, float height,
                    std::shared_ptr<VROVideoTexture> texture);
    
    std::shared_ptr<VROVideoTexture> _texture;
    
};

#endif /* VROVideoSurface_h */
