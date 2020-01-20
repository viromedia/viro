//
//  VROTaskQueue.h
//  ViroKit
//
//  Created by Raj Advani on 3/7/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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
