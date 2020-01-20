//
//  VROProjector.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROProjector_h
#define VROProjector_h

#include "VROVector3f.h"

class VROProjector {
    
public:
    
    /*
     Project the given world coordinates into screen coordinates.
     */
    static bool project(const VROVector3f pos, const float *mvp, const int *viewport, VROVector3f *result);
    
    /*
     Unproject the given screen coordinates into world coordinates. The given screen coordinate vector must
     contain a Z element in the range [0,1], where 0 is the near clipping plane and 1 the far clipping plane.
     */
    static bool unproject(const VROVector3f screen, const float *mvp, const int *viewport, VROVector3f *result);
    
};

#endif /* VROProjector_h */
