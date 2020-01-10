//
//  VROTransformDelegateiOS
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights eserved.
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

#ifndef VROTransformDelegateiOS_h
#define VROTransformDelegateiOS_h

#include <memory>
#include <vector>

@protocol VROTransformDelegateProtocol<NSObject>
@required
- (void)onPositionUpdate:(VROVector3f)position;
@end

/*
 Notifies the bridge regarding transformation udpates of this delegate's attached control
 */
class VROTransformDelegateiOS: public VROTransformDelegate {
public:
    VROTransformDelegateiOS(id<VROTransformDelegateProtocol> delegate, double distanceFilter): VROTransformDelegate(distanceFilter) {
        _delegate = delegate;
    }
    
    virtual ~VROTransformDelegateiOS() {}
    void onPositionUpdate(VROVector3f position) {
        [_delegate onPositionUpdate:position];
    }
    
private:
    __weak id<VROTransformDelegateProtocol> _delegate;
    
};

#endif
