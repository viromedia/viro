//
//  VROARObjectTargetiOS.h
//  ViroKit
//
//  Created by Andy Chu on 8/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARObjectTargetiOS_h
#define VROARObjectTargetiOS_h

#include "VROARObjectTarget.h"

@class ARReferenceObject;

class VROARObjectTargetiOS : public VROARObjectTarget {
    
public:
    VROARObjectTargetiOS(NSURL *localFileUrl);

    virtual ~VROARObjectTargetiOS();

    NSURL *getLocalFileUrl() {
        return _localFileUrl;
    }

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 120000
    ARReferenceObject *getARReferenceObject();
#endif

private:
    NSURL *_localFileUrl;
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 120000
    ARReferenceObject *_referenceObject;
#endif
};


#endif /* VROARObjectTargetiOS_h */
