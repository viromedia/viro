//
//  VROHoverDistanceListener.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/1/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROHoverDistanceListener_h
#define VROHoverDistanceListener_h

#include <stdio.h>

/*
 Attaches to a VROHoverController and receives a notification each
 time the hover distance changes (the distance of the point on the object
 hovered from the camera).
 */
class VROHoverDistanceListener {
    
public:
    
    virtual void onHoverDistanceChanged(float distance, const VRORenderContext &context) = 0;
    
};

#endif /* VROHoverDistanceListener_h */
