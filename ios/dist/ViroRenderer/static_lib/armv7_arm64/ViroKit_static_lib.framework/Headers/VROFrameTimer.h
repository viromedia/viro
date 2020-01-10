//
//  VROFrameTimer.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/5/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
