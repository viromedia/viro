//
//  VROARPlaneNode.h
//  ViroRenderer
//
//  Created by Andy Chu on 6/17/17.
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

#ifndef VROARPlaneNode_h
#define VROARPlaneNode_h

#include "VROARPlaneAnchor.h"
#include "VROARDeclarativeNode.h"

/*
 This class is a nice container that associates the virtual (VRONode), the real (VROARAnchor) and
 the constraints that bind them together.
 */
class VROARDeclarativePlane : public VROARDeclarativeNode {
public:
    VROARDeclarativePlane(float minWidth, float minHeight, VROARPlaneAlignment alignment) :
        _minWidth(minWidth),
        _minHeight(minHeight),
        _alignment(alignment)
    {}
    
    virtual ~VROARDeclarativePlane() {}
    
    /*
     Returns whether or not the given VROARAnchor fulfills this plane's requirements.
     */
    bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        std::shared_ptr<VROARPlaneAnchor> planeAnchor = std::dynamic_pointer_cast<VROARPlaneAnchor>(candidate->getAnchorForTrackable());
        if (!planeAnchor) {
            return false;
        }
        
        if (planeAnchor->getExtent().x < _minWidth || planeAnchor->getExtent().z < _minHeight) {
            return false;
        }

        /*
         The below alignment logic follows this table:
         
         Anchor Alignment | Android - ViroARPlane | iOS - ViroARPlane
         Horizontal       | n/a                   | Horizontal, Up, Down
         Up               | Up, Horizontal        | n/a
         Down             | Down, Horizontal      | n/a
         Vertical         | Vertical              | Vertical

         */
#if VRO_PLATFORM_IOS
        // For iOS, there's no differentiation between upwards and downwards plane, so
        // they'll match "Horizontal" alignments.
        switch (_alignment) {
            case VROARPlaneAlignment::Horizontal:
            case VROARPlaneAlignment::HorizontalUpward:
            case VROARPlaneAlignment::HorizontalDownward:
                if (planeAnchor->getAlignment() != VROARPlaneAlignment::Horizontal) {
                    return false;
                }
                break;
            case VROARPlaneAlignment::Vertical:
                if (planeAnchor->getAlignment() != VROARPlaneAlignment::Vertical) {
                    return false;
                }
                break;
        }
#elif VRO_PLATFORM_ANDROID
        switch (planeAnchor->getAlignment()) {
            case VROARPlaneAlignment::Horizontal:
            case VROARPlaneAlignment::HorizontalUpward:
                if (_alignment != VROARPlaneAlignment::Horizontal
                    && _alignment != VROARPlaneAlignment::HorizontalUpward) {
                    return false;
                }
                break;
            case VROARPlaneAlignment::HorizontalDownward:
                if (_alignment != VROARPlaneAlignment::Horizontal
                    && _alignment != VROARPlaneAlignment::HorizontalDownward) {
                    return false;
                }
            case VROARPlaneAlignment::Vertical:
                if (_alignment != VROARPlaneAlignment::Vertical) {
                    return false;
                }
            default:
                break;
        }
#endif
        return true;
    }
    
    void setMinWidth(float minWidth) {
        _minWidth = minWidth;
    }
    float getMinWidth() {
        return _minWidth;
    }
    
    void setMinHeight(float minHeight) {
        _minHeight = minHeight;
    }
    float getMinHeight() {
        return _minHeight;
    }

    void setAlignment(VROARPlaneAlignment alignment) {
        _alignment = alignment;
    }
    VROARPlaneAlignment getAlignment() {
        return _alignment;
    }

private:
    float _minWidth;
    float _minHeight;
    VROARPlaneAlignment _alignment;
};
#endif /* VROARPlaneNode_h */
