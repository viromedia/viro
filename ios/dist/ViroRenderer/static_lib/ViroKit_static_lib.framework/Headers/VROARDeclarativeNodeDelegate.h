//
//  VROARNodeDelegate.h
//  ViroKit
//
//  Created by Andy Chu on 7/3/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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

#ifndef VROARNodeDelegate_h
#define VROARNodeDelegate_h

class VROARAnchor;

class VROARDeclarativeNodeDelegate {
public:
    
    VROARDeclarativeNodeDelegate() {}
    virtual ~VROARDeclarativeNodeDelegate() {}

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
