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

/*
 Subclasses of VROThreadRestricted may only be accessed from
 a single thread. They invoke passert_thread() to assert this is
 the case.
 */
class VROThreadRestricted {
    
public:
    
    /*
     Default constructor: restrict this object to the current
     thread.
     */
    VROThreadRestricted();
    
    /*
     Restrict this object to the provided thread.
     */
    VROThreadRestricted(pthread_t thread);
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
    
    pthread_t _restricted_thread;
    
};

#endif /* VROThreadRestricted_h */
