//
//  VROPortalFrame.h
//  ViroKit
//
//  Created by Raj Advani on 8/5/17.
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

#ifndef VROPortalFrame_h
#define VROPortalFrame_h

#include "VRONode.h"

class VROShaderModifier;
class VROLineSegment;
enum class VROFace;

/*
 Portal frames are nodes that contain the geometry representing the
 entrance (or exit) from the portal. We need these for two reasons:
 
 1. They enable us to manipulate the scale and other transformations
    of the portal's entrance without changing the transform of the entire
    VROPortal.
 
 2. VROPortals need their frames dynamically assigned each frame if the
    active portal changes, in order to render entrances where necessary
    and exits where necessary.
 */
class VROPortalFrame : public VRONode {
public:
    
    /*
     Return the shader modifier that is used to render a portal frame to the
     stencil buffer. This modifier discards the non-transparent sections of the
     portal frame, so that only the 'window' gets rendered to the stencil
     buffer.
     */
    static std::shared_ptr<VROShaderModifier> getAlphaDiscardModifier();
    
    VROPortalFrame();
    virtual ~VROPortalFrame();
    
    /*
     If this portal frame is two-sided then this returns the stencil face
     that we should render to the stencil buffer. The other face (the inactive)
     face, will not render to the stencil buffer.
     
     Example:
     
     A --- P --- B
     
     Suppose A and B are two portals separated by portal frame P. Suppose P is
     the 'entrance' to B. Our goal is to have the front face of P display B, and
     the back face display A.
     
     Therefore, if we're "in" portal B, we should render the stencil to the back
     faces of B, and if we're "in" portal A, we should render the stencil to the
     front faces of B. This works because when we don't write to the stencil, we
     simply render the world we're in; so in the former case the front of P will
     render B, and the latter case the back of P will render A.
     */
    VROFace getActiveFace(bool isExit) const;
    VROFace getInactiveFace(bool isExit) const;
    
    /*
     Return true if this portal frame is two-sided. See _twoSided below for
     discussion.
     */
    bool isTwoSided() const {
        return _twoSided;
    }
    void setTwoSided(bool twoSided) {
        _twoSided = twoSided;
    }
    
    /*
     Returns true if this frame intersects the given line segment.
     */
    bool intersectsLineSegment(VROLineSegment segment) const;
    
private:
    
    /*
     Normally, both sides of a portal frame are windows into the same scene. So if
     you are in an AR world and you're looking through a portal into a VR world,
     you can walk around the portal and see the VR world from both sides of the
     portal.
     
     However, it's also useful to sometimes have 2-sided portals: that is,
     a portal which on one side shows the VR world and on the other side the AR
     world. This is particularly useful when we're about to pass through a portal.
     In such cases, we want the portal to show different worlds on each side in
     order to prevent flickering.
     
     Two-sided portals are enabled under the hood by using the front/back stencil
     buffer operations.
     */
    bool _twoSided;
    
};

#endif /* VROPortalFrame_h */
