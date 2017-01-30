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
#include "vr/gvr/capi/include/gvr_audio.h"

class VROSoundGVR : public VROSound {
    
public:
    
    /*
     Construct a new sound from the given path. If given a relative path, the file
     can refer directly to an asset in the /assets directory. The file may also be
     an absolute path or an external URL (set isLocalFile to false);
     */
    VROSoundGVR(std::string path, std::shared_ptr<gvr::AudioApi> gvrAudio,
                    VROSoundType type, bool isLocalFile);
    virtual ~VROSoundGVR();
    
    virtual void play();
    
    virtual void pause();
    
    virtual void setVolume(float volume);
    
    virtual void setMuted(bool muted);
    
    virtual void setLoop(bool loop);
    
    virtual void seekToTime(float seconds);
    
    // Used by SoundField
    virtual void setRotation(VROQuaternion rotation);
    
    // Used by SpatialSound
    virtual void setPosition(VROVector3f position);

    // Used by SpatialSound
    virtual VROVector3f getPosition();

    // Used by SpatialSound
    virtual void setTransformedPosition(VROVector3f transformedPosition);

    // Used by SpatialSound
    virtual void setDistanceRolloffModel(VROSoundRolloffModel model, float minDistance,
                                         float maxDistance);


private:
    
    std::shared_ptr<gvr::AudioApi> _gvrAudio;
    std::string _originalPath;
    std::string _fileName;
    bool _isReady = false;
    gvr::AudioSourceId _audioId = -1;
    gvr_audio_distance_rolloff_type _gvrRolloffType = GVR_AUDIO_ROLLOFF_NONE;
    
    void setProperties();
    void loadSound(std::string path, std::function<void(std::string)> onFinish);

    static std::map<std::string, std::string> _preloadedFiles;
};

#endif /* VROSoundGVR_h */
