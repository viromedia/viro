//
//  VROBodyPlayer.h
//  ViroRenderer
//
//  Copyright © 2019 Viro Media. All rights reserved.
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

#ifndef VROBodyPlayer_h
#define VROBodyPlayer_h

#include <memory>
#include <map>
#include "VROFrameSynchronizer.h"
#include "VROFrameListener.h"
#include "VROBodyTracker.h"
#include "VROBodyAnimData.h"

// Enum values matter representing the current body animations state.
enum class VROBodyPlayerStatus {
    Initialized           = 0, // Signified animation has been prepared and parsed.
    Start                 = 1, // Animation has started.
    Paused                = 2, // Animation is stopped.
    Playing               = 3, // Animation is playing.
    Finished              = 4, // Animation has finished.
};

extern const std::string kBodyAnimTotalTime;
extern const std::string kBodyAnimAnimRows;
extern const std::string kBodyAnimJoints;
extern const std::string kBodyAnimTimestamp;
extern const std::string kBodyAnimInitModelTransform;
extern const std::string kBodyAnimVersion;
extern const std::string kBodyAnimUserBoneLengths;

class VROBodyPlayerDelegate {
public:
    virtual void onBodyPlaybackStarting(std::shared_ptr<VROBodyAnimData> animData) = 0;
    virtual void onBodyJointsPlayback(const std::map<VROBodyJointType, VROVector3f> &joints, VROBodyPlayerStatus status) = 0;
};

/**
   VROBodyPlayer parses and plays plays back an animation that is recorded via the VROBodyController.
 **/

class VROBodyPlayer : public VROFrameListener,
public std::enable_shared_from_this<VROBodyPlayer> {
public:
#pragma mark - Initialization
    VROBodyPlayer() {};
    virtual ~VROBodyPlayer() {}

    /*
     Setup the VROBodyPlayer by attaching it the provided frameSynchronizer. Invoke before start() otherwise animation won't play.
     */
    void initPlayer(std::shared_ptr<VROFrameSynchronizer> frameSynchronizer) {
        frameSynchronizer->removeFrameListener(std::dynamic_pointer_cast<VROBodyPlayer>(shared_from_this()));
        frameSynchronizer->addFrameListener(std::dynamic_pointer_cast<VROBodyPlayer>(shared_from_this()));
    }

#pragma mark - Prepare, play, stop and setTime methods.
    /*
     Prepares animation by taking JSON String and serializing it to NSDictionary objects.
     */
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void setLooping(bool loop) = 0;
    virtual void setTime(double time) = 0;
    /*
     Load an animation from a JSON string. This will reset all play properties for this player to defaults, including looping, time, etc.
     */
    virtual void loadAnimation(std::string jsonAnim) = 0;

#pragma mark - VROFrameListener methods
    virtual void onFrameWillRender(const VRORenderContext &context) = 0;
    virtual void onFrameDidRender(const VRORenderContext &context) = 0;

    // delegate to be invoked as body animation is being played back.
    void setDelegate(std::shared_ptr<VROBodyPlayerDelegate> delegate) {
        _bodyMeshDelegate_w = delegate;
    }

protected:
    std::weak_ptr<VROBodyPlayerDelegate> _bodyMeshDelegate_w;
};

#endif /* VROBodyPlayer_h */
