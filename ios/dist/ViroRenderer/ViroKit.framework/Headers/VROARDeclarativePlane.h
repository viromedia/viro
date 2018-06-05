//
//  VROARPlaneNode.h
//  ViroRenderer
//
//  Created by Andy Chu on 6/17/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
         Vertical         | n/a                   | Vertical

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
        // Android really only has 2 types of plane alignments, up and down.
        switch (planeAnchor->getAlignment()) {
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
