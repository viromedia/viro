//
//  VROFrameTimer.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/5/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROFrameTimer_h
#define VROFrameTimer_h

#include "VROTime.h"

/*
 Startup frames have unlimited processing time; process-only
 frames are frames in which we only process and do not render.
 */
enum class VROFrameType {
    Normal,
    Startup,
    ProcessOnly
};

/*
 Tracks how much time remains in the current frame for processing
 tasks.
 */
class VROFrameTimer {
    
public:
    
    VROFrameTimer(VROFrameType frameType, double timeForProcessing, double lastFrameEndTime) :
        _frameType(frameType),
        _timeForProcessing(timeForProcessing),
        _lastFrameEndTime(lastFrameEndTime) {
    }
    
    /*
     Returns true if there is time remaining for any processing operations in the
     current frame.
     */
    bool isTimeRemainingInFrame() const {
        return _frameType == VROFrameType::Startup || getTimeRemainingInFrame() > 0;
    }
    
    double getTimeRemainingInFrame() const {
        return _timeForProcessing - (VROTimeCurrentMillis() - _lastFrameEndTime);
    }
    
private:
    
    /*
     The frame 'type'. Startup frames have unlimited processing time; process-only
     frames are frames in which we only process() and do not render().
     */
    const VROFrameType _frameType;
    
    /*
     The time available for processing this frame, in ms.
     */
    const double _timeForProcessing;
    
    /*
     The end time of the last frame, in ms.
     */
    const double _lastFrameEndTime;
    
};

#endif /* VROFrameTimer_h */
