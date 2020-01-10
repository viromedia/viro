//
//  VROSound.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/23/16.
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

#ifndef VROSound_h
#define VROSound_h

#include <memory>
#include <stdio.h>
#include <string>
#include "VROSoundDelegateInternal.h"
#include "VROQuaternion.h"
#include "VROModelIOUtil.h"

enum class VROSoundType {
    Normal,
    Spatial,
    SoundField,
};

enum class VROSoundRolloffModel {
    Linear,
    Logarithmic,
    None,
};

class VROSound {
    
public:

    VROSound() : _volume(1), _muted(false), _loop(false) {};
    virtual ~VROSound() {}
    
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setMuted(bool muted) = 0;
    virtual void setLoop(bool loop) = 0;
    virtual void seekToTime(float seconds) = 0;

    virtual void setDelegate(std::shared_ptr<VROSoundDelegateInternal> delegate) {
        _delegate = delegate;
    }

    virtual VROSoundType getType() {
        return _type;
    }

    // Used by SoundField
    virtual void setRotation(VROQuaternion rotation) = 0;

    // Used by SpatialSound
    virtual void setPosition(VROVector3f position) = 0;
    virtual VROVector3f getPosition() = 0;
    virtual void setTransformedPosition(VROVector3f transformedPosition) = 0;

    // Used by SpatialSound
    virtual void setDistanceRolloffModel(VROSoundRolloffModel model, float minDistance,
                                         float maxDistance) = 0;

protected:
    std::shared_ptr<VROSoundDelegateInternal> _delegate;
    float _volume;
    bool _muted;
    bool _loop;
    VROSoundType _type;
    VROQuaternion _rotation;
    VROVector3f _position;
    VROVector3f _transformedPosition;
    VROSoundRolloffModel _rolloffModel;
    float _rolloffMinDistance;
    float _rolloffMaxDistance;
};

#endif /* VROSound_h */
