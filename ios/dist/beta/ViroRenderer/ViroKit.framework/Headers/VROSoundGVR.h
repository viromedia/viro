//
//  VROSoundGVR.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/26/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSoundGVR_h
#define VROSoundGVR_h

#include <functional>
#include <memory>
#include <map>
#include "VROSound.h"
#include "VROSoundData.h"
#include "VROSoundDataDelegate.h"
#include "VROModelIOUtil.h"

namespace gvr {
    class AudioApi;
}

class VROSoundGVR : public VROSound, public VROSoundDataDelegate, public std::enable_shared_from_this<VROSoundGVR> {
    
public:

    /*
     Note: we should use the static factory create methods rather than the constructors, because
     they automatically call the init function (vs having to call it manually ourselves)
     */
    static std::shared_ptr<VROSoundGVR> create(std::string resource, VROResourceType resourceType,
                                               std::shared_ptr<gvr::AudioApi> gvrAudio,
                                               VROSoundType type);
    static std::shared_ptr<VROSoundGVR> create(std::shared_ptr<VROSoundData> data,
                                               std::shared_ptr<gvr::AudioApi> gvrAudio,
                                               VROSoundType type);

    VROSoundGVR(std::string resource, VROResourceType resourceType, std::shared_ptr<gvr::AudioApi> gvrAudio,
                VROSoundType type);
    VROSoundGVR(std::shared_ptr<VROSoundData> data, std::shared_ptr<gvr::AudioApi> gvrAudio,
                VROSoundType type);
    
    virtual ~VROSoundGVR();
    
    virtual void play();
    virtual void pause();
    virtual void setVolume(float volume);
    virtual void setMuted(bool muted);
    virtual void setLoop(bool loop);
    virtual void seekToTime(float seconds);
    virtual void setDelegate(std::shared_ptr<VROSoundDelegateInternal> delegate);
    
    // Used by SoundField
    virtual void setRotation(VROQuaternion rotation);
    
    // Used by SpatialSound
    virtual void setPosition(VROVector3f position);
    virtual VROVector3f getPosition();
    virtual void setTransformedPosition(VROVector3f transformedPosition);
    virtual void setDistanceRolloffModel(VROSoundRolloffModel model, float minDistance, float maxDistance);

    #pragma mark VROSoundDataDelegate Implementation

    void dataIsReady();
    void dataError(std::string error);

private:
    
    /*
     Private methods
     */
    void setup();
    void setProperties();

    /*
     Private fields
     */
    bool _ready = false;
    bool _paused = false;
    std::shared_ptr<VROSoundData> _data;
    std::shared_ptr<gvr::AudioApi> _gvrAudio;
    
    int32_t _audioId = -1; // (type is gvr::AudioSourceId)
    int _gvrRolloffType; // type is gvr_audio_distance_rolloff_type
};

#endif /* VROSoundGVR_h */
