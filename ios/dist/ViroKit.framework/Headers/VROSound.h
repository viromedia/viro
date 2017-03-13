//
//  VROSound.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/23/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSound_h
#define VROSound_h

#include <memory>
#include <stdio.h>
#include <string>
#include "VROSoundDelegateInternal.h"
#include "VROQuaternion.h"

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
