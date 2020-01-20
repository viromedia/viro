//
//  VROHitTestResult.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/13/16.
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

#ifndef VROHitTestResult_h
#define VROHitTestResult_h

#include "VROVector3f.h"
#include "VROCamera.h"
#include <memory>

class VRONode;

class VROHitTestResult {
    
public:
    
    VROHitTestResult(std::shared_ptr<VRONode> node, VROVector3f location, float distance, bool background,
                     const VROCamera &camera) :
        _node(node),
        _location(location),
        _distance(distance),
        _background(background),
        _camera(camera)
    {}
    
    ~VROHitTestResult() {}
    
    std::shared_ptr<VRONode> getNode() const {
        return _node;
    }
    
    VROVector3f getLocation() const {
        return _location;
    }
    
    float getDistance() const {
        return _distance;
    }
    
    bool isBackgroundHit() const {
        return _background;
    }
    
    const VROCamera &getCamera() const {
        return _camera;
    }

private:
    
    std::shared_ptr<VRONode> _node;
    VROVector3f _location;
    float _distance;
    bool _background;
    VROCamera _camera;

};

#endif /* VROHitTestResult_h */
