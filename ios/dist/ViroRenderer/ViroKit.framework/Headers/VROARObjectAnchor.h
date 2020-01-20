//
//  VROARObjectAnchor.h
//  ViroKit
//
//  Created by Andy Chu on 8/8/18.
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

#ifndef VROARObjectAnchor_h
#define VROARObjectAnchor_h

#include "VROARAnchor.h"
#include "VROARObjectTarget.h"

class VROARObjectAnchor : public VROARAnchor {
public:
    VROARObjectAnchor(std::shared_ptr<VROARObjectTarget> objectTarget) :
        _objectTarget(objectTarget) {}

    virtual ~VROARObjectAnchor() {}

    std::shared_ptr<VROARObjectTarget> getObjectTarget() {
        return _objectTarget;
    }
private:
    std::shared_ptr<VROARObjectTarget> _objectTarget;
};

#endif /* VROARObjectAnchor_h */
