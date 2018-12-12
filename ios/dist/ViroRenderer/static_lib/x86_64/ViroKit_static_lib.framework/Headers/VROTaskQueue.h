//
//  VROTaskQueue.h
//  ViroKit
//
//  Created by Raj Advani on 3/7/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROTaskQueue_h
#define VROTaskQueue_h

#include <stdio.h>
#include <queue>
#include <string>
#include <memory>
#include <functional>

#define kDebugTaskQueues 0

/*
 Indicate whether tasks should be run sequentially (serially) or concurrently (in parallel).
 */
enum class VROTaskExecutionOrder {
    Serial,
    Concurrent
};

/*
 Task queue accumulates async tasks on the rendering thread, then signals
 the rendering thread upon the completion of all tasks. This is useful when
 we have to wait on a batch of background tasks to complete (e.g., downloading
 textures) before performing some action.
 */
class VROTaskQueue : public std::enable_shared_from_this<VROTaskQueue> {
public:
    
    VROTaskQueue(std::string name, VROTaskExecutionOrder executionOrder);
    virtual ~VROTaskQueue();
    
    /*
     Add a task to the queue. The task should be some function that executes
     asynchronously then, when done, invokes onTaskComplete() on this queue.
     Tasks cannot be added after processTasksAsync has been invoked.
     
     Must be invoked on the rendering thread.
     */
    void addTask(std::function<void()> task);
    
    /*
     Begin processing all the tasks. Invokes the onFinished callback function on
     the *rendering thread* when all tasks have executed.
     
     Must be invoked on the rendering thread.
     */
    void processTasksAsync(std::function<void()> onFinished);
    
    /*
     Tasks should invoke this method when they are complete.
     
     Must be invoked on the rendering thread.
     */
    void onTaskComplete();

    /*
     Debug method to print the names of task queues that have started but
     not completed. Requires kDebugTaskQueues.
     */
    static void printTaskQueues();
    
private:
    
    bool _started;
    VROTaskExecutionOrder _executionOrder;
    std::string _name;
    std::vector<std::function<void()>> _tasks;
    int _numOpenTasks;
    std::function<void()> _onFinished;
    
};

#endif /* VROTaskQueue_hpp */
