//
//  VROARImageTargetiOS.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
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

#ifndef VROARImageTargetiOS_h
#define VROARImageTargetiOS_h

#include "VROARImageTarget.h"
#include <UIKit/UIKit.h>

@class ARReferenceImage;

class VROARImageTargetiOS : public VROARImageTarget {
public:
    VROARImageTargetiOS(UIImage *sourceImage, VROImageOrientation orientation, float physicalWidth);
    virtual ~VROARImageTargetiOS();

    void initWithTrackingImpl(VROImageTrackingImpl impl);

    UIImage *getSourceImage() {
        return _sourceImage;
    }

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110300
    ARReferenceImage *getARReferenceImage();
#endif
private:
    UIImage *_sourceImage;
    
    VROImageTrackingImpl _currentImpl;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110300
    ARReferenceImage *_referenceImage;
#endif
};


#endif /* VROARImageTargetiOS_h */

