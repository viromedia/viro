//
//  VRORenderDelegateiOS.h
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROEventDelegateiOS_h
#define VROEventDelegateiOS_h
#import "VROEventDelegate.h"
#import <Foundation/Foundation.h>
#import "VROARPointCloud.h"

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
- (void)onDrag:(int)source node:(std::shared_ptr<VRONode>)node posX:(float)x posY:(float)y posZ:(float)z;
- (void)onPinch:(int)source node:(std::shared_ptr<VRONode>)node scaleFactor:(float)scale pinchState:(VROEventDelegate::PinchState)pinchState;
- (void)onRotate:(int)source node:(std::shared_ptr<VRONode>)node rotationRadians:(float)rotationRadians rotateState:(VROEventDelegate::RotateState)rotateState;
- (void)onCameraARHitTest:(std::vector<std::shared_ptr<VROARHitTestResult>>) results;
- (void)onARPointCloudUpdate:(std::shared_ptr<VROARPointCloud>) pointCloud;
- (void)onCameraTransformUpdate:(VROVector3f)position rotation:(VROVector3f)rotation forward:(VROVector3f)forward up:(VROVector3f)up;
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
    
    virtual void onCameraARHitTest(std::vector<std::shared_ptr<VROARHitTestResult>> results) {
        [_delegate onCameraARHitTest:results];
    }
    
    virtual void onDrag(int source, std::shared_ptr<VRONode> node, VROVector3f position) {
        [_delegate onDrag:source node:node posX:position.x posY:position.y posZ:position.z];
    }
    
    virtual void onPinch(int source, std::shared_ptr<VRONode> node, float scale, PinchState pinchState) {
        [_delegate onPinch:source node:node scaleFactor:scale pinchState:pinchState];
    }
    
    virtual void onRotate(int source, std::shared_ptr<VRONode> node, float rotationRadians, RotateState rotateState) {
        [_delegate onRotate:source node:node rotationRadians:rotationRadians rotateState:rotateState];
    }

    virtual void onARPointCloudUpdate(std::shared_ptr<VROARPointCloud> pointCloud) {
        [_delegate onARPointCloudUpdate:pointCloud];
    }

    virtual void onCameraTransformUpdate(VROVector3f position, VROVector3f rotation, VROVector3f forward, VROVector3f up) {
        [_delegate onCameraTransformUpdate:position rotation:rotation forward:forward up:up];
    }

private:
  
    __weak id<VROEventDelegateProtocol> _delegate;
  
};
#endif /* VROEventDelegateiOS_h */
