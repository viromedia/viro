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

    void setPosition(VROVector3f position);
    void setRadius(float radius);
    void setThickness(float thickness);

    void setEnabled(bool enabled);
    void setPointerMode(bool isPointerMode);
    bool getPointerMode();
private:

    /**
     * Set to true by controllers if the reticle can be pointed with (like a laser
     * pointer) - usually used for hand held controllers like daydream.
     * False if we wish to lock the reticle to the center of the screen - usually
     * used for fixed controllers like cardboard.
     */
    bool _isPointerFixed;
    bool _enabled;
    float _size;
    float _thickness;
    float _endThickness;

    std::shared_ptr<VRORenderContext> _context;
    std::shared_ptr<VRONode> _node;
    std::shared_ptr<VROPolyline> _polyline;
    std::vector<VROVector3f> createArc(float radius, int numSegments);
};

#endif