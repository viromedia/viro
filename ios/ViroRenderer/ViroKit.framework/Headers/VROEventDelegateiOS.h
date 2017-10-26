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

/*
 Protocol to be implemented by objective C controls to be
 set on VROEventDelegateiOS for the notification of
 input events.
 */
@protocol VROEventDelegateProtocol<NSObject>
@required
- (void)onHover:(int)source node:(std::shared_ptr<VRONode>)node isHovering:(bool)isHovering hoverLocation:(std::vector<float>)location;
- (void)onClick:(int)source node:(std::shared_ptr<VRONode>)node clickState:(VROEventDelegate::ClickState)clickState clickLocation:(std::vector<float>)location;
- (void)onFuse:(int)source node:(std::shared_ptr<VRONode>)node;
- (void)onDrag:(int)source node:(std::shared_ptr<VRONode>)node posX:(float)x posY:(float)y posZ:(float)y;
- (void)onPinch:(int)source node:(std::shared_ptr<VRONode>)node scaleFactor:(float)scale pinchState:(VROEventDelegate::PinchState)pinchState;
- (void)onRotate:(int)source node:(std::shared_ptr<VRONode>)node rotationFactor:(float)rotation rotateState:(VROEventDelegate::RotateState)rotateState;
- (void)onCameraHitTest:(int)source results:(std::vector<VROARHitTestResult>) results;
@end

/*
 iOS implementation of VROEventDelegate for the notification
 of delegate events across the bridge.
 */
class VROEventDelegateiOS : public VROEventDelegate  {
public:
    VROEventDelegateiOS(id<VROEventDelegateProtocol> delegate) :
        _delegate(delegate) {}
    virtual ~VROEventDelegateiOS() {}

    /*
     Delegate events triggered by the EventManager.
     */
    virtual void onHover(int source, std::shared_ptr<VRONode> node, bool isHovering, std::vector<float> location) {
        [_delegate onHover:source node:node isHovering:isHovering hoverLocation:location];
    }
    
    virtual void onClick(int source, std::shared_ptr<VRONode> node, ClickState clickState, std::vector<float> location) {
        [_delegate onClick:source node:node clickState:clickState clickLocation:location];
    }
    
    virtual void onFuse(int source, std::shared_ptr<VRONode> node, float timeToFuseRatio) {
        /*
         As onFuse is also used by internal components to update ui based
         on timeToFuse ratio, we only want to notify our bridge components
         if we have successfully fused (if timeToFuseRatio has counted down to 0).
         */
        if (timeToFuseRatio > 0.0f){
            return;
        }
        [_delegate onFuse:source node:node];
    }
    
    virtual void onCameraHitTest(int source, std::vector<VROARHitTestResult> results) {
        [_delegate onCameraHitTest:source results:results];
    }
    
    virtual void onDrag(int source, std::shared_ptr<VRONode> node, VROVector3f position) {
        [_delegate onDrag:source node:node posX:position.x posY:position.y posZ:position.z];
    }
    
    virtual void onPinch(int source, std::shared_ptr<VRONode> node, float scale, PinchState pinchState) {
        [_delegate onPinch:source node:node scaleFactor:scale pinchState:pinchState];
    }
    
    virtual void onRotate(int source, std::shared_ptr<VRONode> node, float rotation, RotateState rotateState) {
        [_delegate onRotate:source node:node rotationFactor:rotation rotateState:rotateState];
    }

private:
  
    __weak id<VROEventDelegateProtocol> _delegate;
  
};
#endif /* VROEventDelegateiOS_h */
