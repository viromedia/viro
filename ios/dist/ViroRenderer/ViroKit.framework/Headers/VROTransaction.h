//
//  VROTransaction.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROTransaction_h
#define VROTransaction_h

#include <stdio.h>
#include <vector>
#include <memory>
#include <functional>
#include "VROAnimation.h"
#include "VROTimingFunction.h"

class VROTransaction {

public:
    
#pragma mark - Static Animation API

    /*
     Retrieve the active (most deeply nested) uncommitted VROTransaction. Returns
     nullptr if there is no active VROTransaction.
     */
    static std::shared_ptr<VROTransaction> get();

    /*
     Return true if there exists an uncommitted VROTransaction with duration greater
     than zero. If false, then animations are considered disabled.
     */
    static bool isActive();

    /*
     Update the T values for all committed animation transactions.
     */
    static void update();

    /*
     Begin a new VROTransaction on this thread, and make this the active animation
     transaction.
     */
    static void begin();

    /*
     Adds the given transaction to the openTransactions stack. Internal use only by the Java API.
     Enables creating a new transaction off the rendering thread, before adding it to the animation
     stack on the rendering thread.
     */
    static void add(std::shared_ptr<VROTransaction> transaction);

    /*
     Pauses a VROTransaction if it hasn't yet been paused.
     */
    static void pause(std::shared_ptr<VROTransaction> transaction);

    /*
     Resumes a paused a VROTransaction. The animation will continue to
     play from where it had left off.
     */
    static void resume(std::shared_ptr<VROTransaction> transaction);

    /*
     Cancels a VROTransaction at the current T value. Cancelled transactions
     can no longer be paused or resumed.
     
     Note: this is currently only used internally and currently isn't hooked
     up to animations at all (it won't cancel animations, call callbacks, etc).
     
     TODO: VIRO-1464 Add ability to cancel (vs terminate) animations
     */
    static void cancel(std::shared_ptr<VROTransaction> transaction);

    /*
     Terminates a VROTransaction. Terminated transactions can no longer
     be paused or resumed.
     */
    static void terminate(std::shared_ptr<VROTransaction> transaction, bool jumpToEnd);

    /*
     Commit the active VROTransaction.
     */
    static std::shared_ptr<VROTransaction> commit();

    /*
     Set the animation duration for the active animation transaction, in seconds.
     */
    static void setAnimationDuration(float durationSeconds);
    static float getAnimationDuration();

    /*
     Set the time in seconds that we wait before the animation starts (after
     the animation is committed).
     */
    static void setAnimationDelay(float delaySeconds);

    /*
     Set to true to make this transaction automatically loop to the beginning on finish.
     In effect - reset _time to beginning of animation onFinish if _loop is set.
     */
    static void setAnimationLoop(bool loop);

    /*
     Set a callback to invoke when the active transaction completes (after duration
     seconds).
     */
    static void setFinishCallback(std::function<void(bool terminate)> finishCallback);

    /*
     Set a timing function, which defines the curve of the animation (ease in, ease out,
     etc.)
     */
    static void setTimingFunction(VROTimingFunctionType timingFunctionType);
    static void setTimingFunction(std::unique_ptr<VROTimingFunction> timingFunction);
    
#pragma mark - VROTransaction

    VROTransaction();
    ~VROTransaction() {}

    /*
     Get the T value (between 0 and 1) representing current progress through this
     animation.
     */
    double getT() {
        return _t;
    }

    /*
     Return true if this animation has a duration of 0.
     */
    bool isDegenerate() {
        return _durationSeconds <= 0;
    }

    /*
     Add a new animation to this transaction.
     */
    void addAnimation(std::shared_ptr<VROAnimation> animation) {
        _animations.push_back(animation);
    }

    /*
     Process another frame of all animations in this transaction. The
     t value here is prior to timing function transformation.
     */
    void processAnimations(float t);

    /*
     Invoked when the transaction is finished.
     */
    void onTermination();

private:

    double _t;
    double _durationSeconds;
    double _startTimeSeconds;
    double _delayTimeSeconds;
    double _processedTimeWhenPaused;
    bool _paused;
    bool _loop;
    std::unique_ptr<VROTimingFunction> _timingFunction;

    std::function<void(bool terminate)> _finishCallback;
    std::vector<std::shared_ptr<VROAnimation>> _animations;

};

#endif /* VROTransaction_h */
