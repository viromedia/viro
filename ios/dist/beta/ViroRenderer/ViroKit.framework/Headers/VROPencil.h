//
//  VROPencil.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
    VROPencil(){}
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

private:
    std::vector<std::vector<VROVector3f>> _paths;
};

#endif
