//
//  VROThreadRestricted.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/30/17.
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
    void passert_thread(std::string method);
    
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
