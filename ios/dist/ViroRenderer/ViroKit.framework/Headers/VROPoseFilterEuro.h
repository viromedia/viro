//
//  VROPoseFilterEuro.h
//  ViroKit
//
//  Created by Raj Advani on 3/1/19.
//  Copyright © 2019 Viro Media. All rights reserved.
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

#ifndef VROPoseFilterEuro_h
#define VROPoseFilterEuro_h

#include "VROPoseFilter.h"
#include "VROOneEuroFilter.h"

class VROPoseFilterEuro : public VROPoseFilter {
public:
    
    VROPoseFilterEuro(float trackingPeriodMs, float confidenceThreshold);
    virtual ~VROPoseFilterEuro();
    
    VROPoseFrame temporalFilter(const std::vector<VROPoseFrame> &frames, const VROPoseFrame &combinedFrame,
                                const VROPoseFrame &newFrame);
    
    /*
     Set the parameters to use for *all* joints. See VROPoseFilterEuro.cpp top-notes
     for a description of these parameters.
     */
    void setBeta(float beta);
    void setFCMin(float fcMin);
    
private:
    std::vector<std::shared_ptr<VROOneEuroFilter>> _filters;
    
};

#endif /* VROPoseFilterEuro_h */
