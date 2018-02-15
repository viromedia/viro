//
//  VROPortalDelegate.hpp
//  ViroRenderer
//
//  Created by Vik Advani on 9/14/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPortalDelegate_h
#define VROPortalDelegate_h

#include <memory>
#include <vector>
#include "VROVector3f.h"
#include "VROTime.h"

/*
 Delegate that is attached to a node for notifying the bridge regarding transformation updates.
 */
class VROPortalDelegate {
public:
    VROPortalDelegate() {}
    virtual ~VROPortalDelegate() {}
    virtual void onPortalEnter()=0;
    virtual void onPortalExit()=0;
};
#endif
