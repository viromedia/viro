//
//  VROARDeclarativeImageNode.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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

#ifndef VROARDeclarativeImageNode_h
#define VROARDeclarativeImageNode_h

#include "VROARImageAnchor.h"
#include "VROARDeclarativeNode.h"
#include "VROARImageTarget.h"

class VROARDeclarativeImageNode : public VROARDeclarativeNode {
public:

    VROARDeclarativeImageNode() {}
    virtual ~VROARDeclarativeImageNode() {}
    
    /*
     Returns whether or not the given VROARAnchor fulfills this image marker's requirements.
     */
    bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        std::shared_ptr<VROARImageAnchor> imageAnchor = std::dynamic_pointer_cast<VROARImageAnchor>(candidate->getAnchorForTrackable());
        // a VROARImageAnchor matches an VROAR*ImageNode if they share the same VROImageTarget
        return imageAnchor && _imageTarget == imageAnchor->getImageTarget();
    }

    void setImageTarget(std::shared_ptr<VROARImageTarget> imageTarget) {
        _imageTarget = imageTarget;
    }

    std::shared_ptr<VROARImageTarget> getImageTarget() {
        return _imageTarget;
    }
    
private:
    std::shared_ptr<VROARImageTarget> _imageTarget;
};

#endif /* VROARDeclarativeImageNode_h */
