//
//  VROARNodeDelegate.h
//  ViroKit
//
//  Created by Andy Chu on 7/3/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARNodeDelegate_h
#define VROARNodeDelegate_h

class VROARAnchor;

class VROARNodeDelegate {
public:
    
    VROARNodeDelegate() {}
    virtual ~VROARNodeDelegate() {}

    /*
     Notifies the delegate that the node was attached to the given anchor. The
     delegate should NOT hold onto the anchor.
     */
    virtual void onARAnchorAttached(std::shared_ptr<VROARAnchor> anchor) = 0;
    
    /*
     Notifies the delegate that the underlying anchor was updated. The
     delegate should NOT hold onto the anchor.
     */
    virtual void onARAnchorUpdated(std::shared_ptr<VROARAnchor> anchor) = 0;
    
    /*
     Notifies the delegate that the underlying anchor was removed.
     */
    virtual void onARAnchorRemoved() = 0;
};

#endif /* VROARNodeDelegate_h */
