//
//  VROReticle.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/12/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VRORecticle_h
#define VRORecticle_h

#include <memory>
#include <vector>

class VRONode;
class VROPolyline;
class VROVector3f;
class VRORenderContext;
class VRODriver;
enum class VROEyeType;

class VROReticle {

public:

    VROReticle();
    virtual ~VROReticle();

    void trigger();
    void renderEye(VROEyeType eye, const VRORenderContext *renderContext, VRODriver *driver);

    void setDepth(float depth);
    void setRadius(float radius);
    void setThickness(float thickness);

    void setEnabled(bool enabled);

private:

    bool _enabled;
    float _size;
    float _thickness;
    float _endThickness;

    std::shared_ptr<VRONode> _node;
    std::shared_ptr<VROPolyline> _polyline;

    std::vector<VROVector3f> createArc(float radius, int numSegments);

};

#endif