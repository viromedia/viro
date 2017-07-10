//
//  VROARNodeDelegateiOS.h
//  ViroKit
//
//  Created by Andy Chu on 7/3/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARNodeDelegateiOS_h
#define VROARNodeDelegateiOS_h

#import "VROARNodeDelegate.h"
#import <Foundation/Foundation.h>

@protocol VROARNodeDelegateProtocol<NSObject>
@required
- (void)onARAnchorAttached:(std::shared_ptr<VROARAnchor>) anchor;
- (void)onARAnchorUpdated:(std::shared_ptr<VROARAnchor>) anchor;
- (void)onARAnchorRemoved;
@end

/**
 * iOS implementation of the VROARNodeDelegate
 */
class VROARNodeDelegateiOS : public VROARNodeDelegate {
public:
    VROARNodeDelegateiOS(id<VROARNodeDelegateProtocol> delegate) :
        _delegate(delegate) {}

    virtual ~VROARNodeDelegateiOS() {}
    
    virtual void onARAnchorAttached(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onARAnchorAttached:anchor];
    }
    
    virtual void onARAnchorUpdated(std::shared_ptr<VROARAnchor> anchor) {
        [_delegate onARAnchorUpdated:anchor];
    }
    
    virtual void onARAnchorRemoved() {
        [_delegate onARAnchorRemoved];
    }
    
private:
    __weak id<VROARNodeDelegateProtocol> _delegate;
    
};
#endif /* VROARNodeDelegateiOS_h */

