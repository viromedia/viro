//
//  VROFrameScheduler.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/5/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROFrameScheduler_h
#define VROFrameScheduler_h

#include "VROFrameTimer.h"
#include <functional>
#include <queue>
#include <mutex>
#include <string>
#include <set>

struct VROFrameTask {
    std::string key;
    std::function<void()> functor;
};

/*
 Schedules tasks to be run on the rendering thread. Unlike 
 VROPlatformDispatchRenderer, these tasks are run in a time-limited
 queue; they are scheduled to run only when time is available in
 the current frame. Time remaining in a frame is determined by a
 set milliseconds-per-frame (mpf) target.
 */
class VROFrameScheduler {
    
public:
    
    VROFrameScheduler();
    virtual ~VROFrameScheduler();
    
    /*
     Return true if the given task is already queued.
     */
    bool isTaskQueued(std::string key);
    
    /*
     Schedule a new task to be completed in the time-limited 
     queue. The key should uniquely identify the task, and is used
     to de-dupe tasks that are scheduled multiple times.
     */
    void scheduleTask(std::string key, std::function<void()> task);
    
    /*
     Process as many tasks as allowed given the remaining frame
     time.
     */
    void processTasks(const VROFrameTimer &timer);
    
private:
    
    /*
     The number of frames that have passed, during which we had at
     lease one task to process but no time to process any.
     */
    int _starvationFrameCount;
    
    /*
     Guards the _taskQueue and _queuedTasks set.
     */
    std::recursive_mutex _taskQueueMutex;
    
    /*
     The actual queue we use to process tasks in FIFO order.
     */
    std::queue<VROFrameTask> _taskQueue;
    
    /*
     Set used to prevent the same task from being queued
     multiple times, based on its ID.
     */
    std::set<std::string> _queuedTasks;
    
};

#endif /* VROFrameScheduler_h */
