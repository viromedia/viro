//
//  VROVideoTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoTexture_h
#define VROVideoTexture_h

#include "VROFrameListener.h"
#include "VROTexture.h"
#include <memory>
#include "VROVideoDelegateInternal.h"
#include "VROTime.h"
#include <math.h>       /* floor */

class VRORenderContext;
class VROFrameSynchronizer;
class VRODriver;

class VROVideoTexture : public VROTexture, public VROFrameListener, public std::enable_shared_from_this<VROVideoTexture> {
    
public:
    
    VROVideoTexture(VROTextureType type) :
        VROTexture(type) {}
    virtual ~VROVideoTexture() {}
    
    /*
     Use this video texture to display the contents of the given URL. The video
     will not run until play() is invoked.
     */
    virtual void loadVideo(std::string url,
                           std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                           VRODriver &driver) = 0;
    
    /*
     Perform video initialization (which causes a stutter) early.
     */
    virtual void prewarm() = 0;
    
    virtual void onFrameWillRender(const VRORenderContext &context) {
        if (_delegate != nullptr && !isPaused()) {
            updateVideoTime();
        }
    }
    virtual void onFrameDidRender(const VRORenderContext &context) {}
    
    virtual void pause() = 0;
    virtual void play() = 0;
    virtual bool isPaused() = 0;

    virtual void seekToTime(int seconds) = 0;
    virtual int getCurrentTimeInSeconds() = 0;
    virtual int getVideoDurationInSeconds() = 0;

    virtual void setMuted(bool muted) = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setLoop(bool loop) = 0;
    
    virtual void setDelegate(std::shared_ptr<VROVideoDelegateInternal> delegate) {
        _delegate = delegate;
    }
    
protected:
    std::shared_ptr<VROVideoDelegateInternal> _delegate;

    /*
     * Notifies delegates about the video player's current time, per second.
     */
    void updateVideoTime(){
        double currentTime = floor(VROTimeCurrentSeconds());

        /*
         * _lastVideoTimeGetAttempt is used to reduce the amount of JNI
         * calls (getCurrentTimeInSeconds()) to be queried every
         * second - the lowest unit of time currently used by video players.
         */
        if (_lastVideoTimeGetAttempt != currentTime) {

            _lastVideoTimeGetAttempt = currentTime;
            int currentVideoTimeInSeconds = getCurrentTimeInSeconds();

            // Only notify delegates if the last known CurrentVideoTime has changed.
            if (_lastCurrentVideoTimeInSeconds != currentVideoTimeInSeconds) {
                _delegate->onVideoUpdatedTime(currentVideoTimeInSeconds,
                                              getVideoDurationInSeconds());
                _lastCurrentVideoTimeInSeconds = currentVideoTimeInSeconds;
            }
        }
    }

private:
    /*
     * Last time stamp at which we attempted a JNI call getCurrentTimeInSeconds() from the
     * video player.
     */
    double _lastVideoTimeGetAttempt  = -1;

    /*
     * Last known current video time that was retrieved from the player.
     */
    int _lastCurrentVideoTimeInSeconds = -1;
};

#endif /* VROVideoTexture_h */
