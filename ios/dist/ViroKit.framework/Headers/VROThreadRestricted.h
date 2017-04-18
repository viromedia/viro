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
 Subclasses of VROThreadRestricted may only be accessed from
 a single thread. The thread is identified either by its VROThreadName,
 or by an exact pthread. The passert_thread() is provided to assert
 we're on the thread to which we're restricted.
 */
class VROThreadRestricted {
    
public:

    /*
     Associate the given VROThread with the given pthread.
     */
    static void setThread(VROThreadName name, pthread_t thread);

    /*
     Get the pthread with the given name. Returns ret_not_found if there is
     no thread with the given name.
     */
    static pthread_t getThread(VROThreadName name, pthread_t ret_not_found);
    
    /*
     Default constructor: restrict this object to the current
     thread.
     */
    VROThreadRestricted();
    
    /*
     Restrict this object to the provided thread.
     */
    VROThreadRestricted(pthread_t thread);

    /*
     Restrict this object to the thread with the given name. The
     name must have been set to a thread via
     setThread(VROThreadName, pthread_t).
     */
    VROThreadRestricted(VROThreadName name);

    virtual ~VROThreadRestricted();
    
    /*
     Set the thread to which this object is restricted. This 
     is used when the thread is not available for the constructor.
     */
    void setThreadRestriction(pthread_t thread);
    
    /*
     Assert we are on the correct thread. If not, abort.
     */
    void passert_thread();
    
private:

    /*
     Only one of these will be set for a given thread-restricted
     object.
     */
    VROThreadName _restricted_thread_name;
    pthread_t _restricted_thread;
    
};

#endif /* VROThreadRestricted_h */
