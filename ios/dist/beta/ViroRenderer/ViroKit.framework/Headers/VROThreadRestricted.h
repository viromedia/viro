//
//  VROThreadRestricted.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/30/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROThreadRestricted_h
#define VROThreadRestricted_h

#include <stdio.h>
#include <pthread.h>
#include <string>

// Utility for printing out an ID for the given pthread
std::string print_thread_id(pthread_t id);

enum class VROThreadName {
    Undefined,
    Renderer
};

/*
 Subclasses of VROThreadRestricted may only be accessed from a single thread,
 identified by its VROThreadName.
 
 The passert_thread() method is provided to assert we're on the thread to which
 we're restricted.
 */
class VROThreadRestricted {
    
public:

    /*
     Associate the current thread with the given VROThreadName.
     */
    static void setThread(VROThreadName name);
    static void unsetThread();
    static bool isThread(VROThreadName name);

    /*
     Restrict this object to the thread with the given name. The name must have been set
     to a thread via setThread(VROThreadName, pthread_t).
     */
    VROThreadRestricted(VROThreadName name);
    virtual ~VROThreadRestricted();
    
    /*
     Assert we are on the correct thread. If not, abort.
     */
    void passert_thread();
    
    /*
     Temporarily enable or disable thread checking. When false, passert_thread() is a no-op.
     */
    void setThreadRestrictionEnabled(bool enabled) {
        _enabled = enabled;
    }
    
private:

    /*
     The name of the thread this object is restricted to.
     */
    VROThreadName _restricted_thread_name;

    /*
     True if thread restriction checking is enabled for this object.
     When false, passert_thread is a no-op.
     */
    bool _enabled;
    
};

#endif /* VROThreadRestricted_h */
