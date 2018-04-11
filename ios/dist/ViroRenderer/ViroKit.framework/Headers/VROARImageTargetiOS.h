//
//  VROARImageTargetiOS.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

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

