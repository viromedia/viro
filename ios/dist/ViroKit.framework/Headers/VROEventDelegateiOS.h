//
//  VRORenderDelegateiOS.h
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROEventDelegateiOS_h
#define VROEventDelegateiOS_h
#import "VROEventDelegate.h"
#import <Foundation/Foundation.h>

/**
 * Protocol to be implemented by objective C controls to be
 * set on VROEventDelegateiOS for the notification of
 * input events.
 */
@protocol VROEventDelegateProtocol<NSObject>
@required
- (void)onHover:(int)source isHovering:(bool)isHovering;
- (void)onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState;
- (void)onFuse:(int)source;
- (void)onDrag:(int)source posX:(float)x posY:(float)y posZ:(float)y;
@end

/**
 * iOS implementation of VROEventDelegate for the notification
 * of delegate events across the bridge.
 */
class VROEventDelegateiOS : public VROEventDelegate  {
public:
    VROEventDelegateiOS(id<VROEventDelegateProtocol> delegate) :
        _delegate(delegate) {}
    virtual ~VROEventDelegateiOS() {}

    /*
     * Delegate events triggered by the EventManager.
     */
    virtual void onHover(int source, bool isHovering) {
        [_delegate onHover:source isHovering:isHovering];
    }
    virtual void onClick(int source, ClickState clickState) {
        [_delegate onClick:source clickState:clickState];
    }
    virtual void onFuse(int source, float timeToFuseRatio) {
        /**
         * As onFuse is also used by internal components to update ui based
         * on timeToFuse ratio, we only want to notify our bridge components
         * if we have successfully fused (if timeToFuseRatio has counted down to 0).
         */
        if (timeToFuseRatio > 0.0f){
            return;
        }
        [_delegate onFuse:source];
    }
    
    virtual void onDrag(int source, float x, float y, float z) {
        [_delegate onDrag:source posX:x posY:y posZ:z];
    }

private:
  
    __weak id<VROEventDelegateProtocol> _delegate;
  
};
#endif /* VROEventDelegateiOS_h */
