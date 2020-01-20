//
//  VROPencil.h
//  ViroRenderer
//
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

#ifndef VROPencil_h
#define VROPencil_h

#include <memory>
#include <vector>
#include "VROSortKey.h"

class VRONode;
class VRORenderContext;
class VRODriver;
class VROVector3f;
class VROVector4f;
class VROMaterial;

/*
 Stored in VRORenderContext, VROPencil is used to draw a list of VROPolylines
 in a separate render pass, after having rendered the scene, mainly for drawing
 debug information.
 */
class VROPencil {
public:
    VROPencil() {
        _brushThickness = 0.05f;
    }
    virtual ~VROPencil();

    /*
     Adds a line to be drawn starting and ending at the provided world coordinates.
     */
    void draw(VROVector3f from, VROVector3f to);

    /*
     Renders the geometry of all lines added with VROPencil.draw(), called in VRORenderer.renderEye(),
     after the scene and reticle has been rendered.
     */
    void render(const VRORenderContext &context, std::shared_ptr<VRODriver> &driver);

    /*
     Clears all added lines in preparation for the next render pass.
     */
    void clear();

    /*
     Sets the thickness of the polyline for this VROPencil during a debug draw.
     */
    void setBrushThickness(float thickness) {
        _brushThickness = thickness;
    }

private:
    std::vector<std::vector<VROVector3f>> _paths;
    float _brushThickness;
};

#endif
